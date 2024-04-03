import Module from './bin/cpp-core';

const editor = document.createElement('div');
editor.id = 'editor';
editor.innerHTML = 'Type here';
editor.style.position = 'fixed';
editor.style.top = '0';
editor.style.left = '0';
editor.style.zIndex = '1000';
editor.style.width = '100000px';
editor.style.color = 'rgba(0, 30, 200, .7)';
editor.style.transformOrigin = '0 0';
editor.contentEditable = 'true';
document.body.appendChild(editor);

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

document.addEventListener('wheel', e => {
	e.preventDefault();

	if (e.ctrlKey) {
		m._cameraZoom(e.x * devicePixelRatio, e.y * devicePixelRatio, e.deltaY * devicePixelRatio);
	} else {
		m._cameraMove(e.deltaX * devicePixelRatio, e.deltaY * devicePixelRatio);
	}
}, { passive: false });
