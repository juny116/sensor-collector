import mysql from 'mysql2/promise';
import { createRepository } from './database.mjs';
import { createApp } from './app.mjs';

if (!process.env.MYSQL_URL) throw new Error('Set MYSQL_URL');
const pool = mysql.createPool(process.env.MYSQL_URL);
const server = createApp(createRepository(pool)).listen(Number(process.env.PORT ?? 8000), process.env.HOST ?? '127.0.0.1');
for (const signal of ['SIGINT', 'SIGTERM']) process.on(signal, () => server.close(async () => { await pool.end(); }));
