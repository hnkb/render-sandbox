import Module from './bin/cpp-core';

const m = await Module();
console.log(m);

const p = document.querySelector('#root');
if (p)
	p.textContent = 'Hello from Typescript app!';
