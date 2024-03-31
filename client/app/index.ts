import Module from './bin/cpp-core';

const m = await Module();


const devicePixelRatio = window.devicePixelRatio || 1;

function resizeCanvas() {
	const width = window.innerWidth * devicePixelRatio;
	const height = window.innerHeight * devicePixelRatio;

	const canvas = document.querySelector('canvas.main') as HTMLCanvasElement;

	// Set the display size of the canvas.
	canvas.style.width = window.innerWidth + 'px';
	canvas.style.height = window.innerHeight + 'px';
	// Set the size of the drawingBuffer
	canvas.width = width;
	canvas.height = height;

	m._resizeCanvas(width, height);
}

resizeCanvas();

window.addEventListener('resize', resizeCanvas);
