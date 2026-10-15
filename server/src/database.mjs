export function createRepository(pool) {
  return {
    async insertSensor(record) { await pool.query('INSERT INTO sensors SET ?', record); },
    async insertTemperature(record) { await pool.query('INSERT INTO temperature SET ?', record); },
    async recentTemperatures() { const [rows] = await pool.query('SELECT * FROM temperature ORDER BY id DESC LIMIT 256'); return rows; },
    async sensors() { const [rows] = await pool.query('SELECT * FROM sensors ORDER BY id'); return rows; }
  };
}
