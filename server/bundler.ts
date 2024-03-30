import { watch } from 'fs';
import { join, parse } from 'path';

// An alternative to this live bundler is to set up scripts in package.json to run the bundler:
//   "build": "bun build client/app/index.ts --outdir client/public/app --watch --sourcemap=external --minify",
//   "dev": "bun run build & bun --hot run server/index.ts"
//
// The alternative approach has two drawbacks:
//   1. the & operator is not cross-platform *cough* Windows *cough*
//   2. the default bun behaviour does not properly append sourcemap name to the output file

const sourceDir = join(import.meta.dir, '../client/app');
const outputDir = join(import.meta.dir, '../client/public/app');

async function build() {
	const bundle = await Bun.build({
		entrypoints: [join(sourceDir, 'index.ts')],
		outdir: outputDir,
		sourcemap: 'external',
		minify: true,
	});

	await Bun.write(join(outputDir, 'cpp-core.wasm'), Bun.file(join(sourceDir, 'bin/cpp-core.wasm')));

	if (bundle.success)
		for (const file of bundle.outputs.filter(e => e.kind === 'entry-point'))
			await Bun.write(file.path, `${await file.text()}\n//# sourceMappingURL=${parse(file.path).base}.map`);
}

export default async function bundler() {
	const watcher = watch(sourceDir, { recursive: true }, async (event, filename) => {
		await build();
		console.log(`Javascript bundle rebuilt because of ${event} in ${filename}`);
	});

	process.on("SIGINT", () => {
		watcher.close();
		process.exit();
	});

	return build();
}
