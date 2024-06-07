import Module from './bin/cpp-core';

window.templateText = 'Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit,\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.'.split('\n')

const textTemplate = document.createElement('canvas');
textTemplate.id = 'textTemplate';
textTemplate.style.position = 'fixed';
textTemplate.style.top = '0';
textTemplate.style.left = '0';
textTemplate.style.zIndex = '1000';

document.body.appendChild(textTemplate);

async function loadFont() {
    const font = new FontFace('ZXX Noise', 'url(/fonts/IBMPlexSansArabic-Regular.ttf)');
    await font.load();
    document.fonts.add(font);
    console.log('Font loaded');
}

await loadFont()

const SPAN = document.createElement('span')
SPAN.innerHTML = 'Hg'

const BLOCK = document.createElement('div')
BLOCK.classList.add('editor_calc_getTextHeight')
BLOCK.style.display = 'inline-block'
BLOCK.style.width = '1px'
BLOCK.style.height = '0px'

window.getFontMetrics = function (
	font: string,
	size: number,
): IFontMetric {
	const result = {
		baselineOffset: 0,
		height: 0,
		descent: 0,
	}
	SPAN.style.fontFamily = font
	SPAN.style.fontSize = `${size}px`

	SPAN.style.lineHeight = ''
	SPAN.style.display = 'inline'

	const text = SPAN
	const block = BLOCK
	const div = document.createElement('div')
	div.appendChild(text)
	div.appendChild(block)

	document.body.appendChild(div)

	try {
		block.style.verticalAlign = 'baseline'
		const offsetBaseline = block.getBoundingClientRect()
		const offsetText = text.getBoundingClientRect()
		if (offsetBaseline !== undefined && offsetText !== undefined) {
			result.baselineOffset = offsetBaseline.top - offsetText.top
		} else {
			throw ''
		}

		block.style.verticalAlign = 'bottom'
		const offsetBottom = block.getBoundingClientRect()
		if (offsetBottom !== undefined && offsetText !== undefined) {
			result.height = offsetBottom.top - offsetText.top
		} else {
			throw ''
		}

		result.descent = result.height - result.baselineOffset
	} finally {
		if (div.parentElement) {
			div.parentElement.removeChild(div)
		}
	}

	return result
}

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

	// template as well
	textTemplate.style.width = window.innerWidth + "px";
	textTemplate.style.height = window.innerHeight + "px";
	textTemplate.width = width;
	textTemplate.height = height;
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
