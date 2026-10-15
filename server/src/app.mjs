import express from 'express';

export function csv(rows, fields) {
  const cell = value => `"${String(value ?? '').replaceAll('"', '""')}"`;
  return [fields.map(cell).join(','), ...rows.map(row => fields.map(key => cell(row[key])).join(','))].join('\n');
}

export function createApp(repository) {
  const app = express();
  const route = fn => (req, res, next) => Promise.resolve(fn(req, res)).catch(next);
  app.get('/', (_req, res) => res.send('Sensor collector'));
  app.get('/updateIOT', route(async (req, res) => {
    const { device, unit, type, value, seq } = req.query;
    if (![device, unit, type, value, seq].every(v => typeof v === 'string') || value.trim() === '' || !Number.isFinite(Number(value))) return res.status(400).json({error: 'Invalid sensor record'});
    await repository.insertSensor({device, unit, type, value: Number(value), seq, ip: req.ip});
    res.json({ok: true});
  }));
  app.get('/update', route(async (req, res) => {
    const value = req.query.value ?? req.query.field1;
    if (typeof value !== 'string' || value.trim() === '' || !Number.isFinite(Number(value))) return res.status(400).json({error: 'Invalid temperature'});
    await repository.insertTemperature({value: Number(value), ip: req.ip});
    res.json({ok: true});
  }));
  app.get('/temperature', route(async (_req, res) => res.json(await repository.recentTemperatures())));
  app.get('/download', route(async (_req, res) => {
    res.type('text/csv').attachment('sensors.csv').send(csv(await repository.sensors(), ['id','device','unit','type','value','seq','ip','time']));
  }));
  app.use((_error, _req, res, _next) => res.status(500).json({error: 'Storage operation failed'}));
  return app;
}
