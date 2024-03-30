import { join } from 'path';
import bundler from './bundler';

bundler();

const server = Bun.serve({
	port: 80,
	async fetch(req) {
		const path = new URL(req.url).pathname;

		if (path === '/')
			return new Response(Bun.file(join(import.meta.dir, '../client/public/index.html')));

		const file = Bun.file(join(import.meta.dir, "../client/public", path));
		if (file.size > 0)
			return new Response(file);

		return new Response('404 Not Found', { status: 404, statusText: 'Not Found' });
	}
})

console.log(`Listening on ${server.url}`);
