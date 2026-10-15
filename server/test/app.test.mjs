import test from 'node:test';
import assert from 'node:assert/strict';
import { once } from 'node:events';
import { createApp, csv } from '../src/app.mjs';

test('CSV quotes values and escapes embedded quotes',()=> {
 assert.equal(csv([{value:'a,"b'}],['value']), '"value"\n"a,""b"');
});
test('sensor routes validate inputs and stream downloads without disk files',async()=> {
 const records=[]; const repo={insertSensor:async x=>records.push(x), insertTemperature:async x=>records.push(x),recentTemperatures:async()=>records,sensors:async()=>records};
 const server=createApp(repo).listen(0,'127.0.0.1'); await once(server,'listening');
 try {
  const base=`http://127.0.0.1:${server.address().port}`;
  assert.equal((await fetch(base+'/update?value=bad')).status,400);
  assert.equal((await fetch(base+'/update?value=')).status,400);
  assert.equal((await fetch(base+'/update?field1=23.5')).status,200);
  assert.equal(records.length,1); assert.equal(records[0].value,23.5);
  const download=await fetch(base+'/download'); assert.match(download.headers.get('content-type'),/text\/csv/); assert.match(await download.text(),/23.5/);
 } finally {await new Promise(resolve=>server.close(resolve));}
});
