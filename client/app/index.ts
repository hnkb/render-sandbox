import Module, { type Mesh, type ShapedGlyph, type Font as NativeFont } from './bin/cpp-core';

const m = await Module();


type Font = NativeFont & {
	meshes: Mesh[];
}

const font = new m.Font("OpenSans-Regular") as Font;
font.meshes = font.getMeshesArray();

console.log(font.vertexData);
console.log(font.indexData);
console.log(font.meshes);
const shaped = font.shape("Hello")
console.log(shaped);
console.log(shaped[0].pos);
font.delete();
