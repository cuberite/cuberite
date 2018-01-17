
const g_DistanceBetweenSquares = 0;//.01;
const g_Colors = [
	"#0000FF",
	"#00FF00",
	"#FF0000",
	"#FF00FF",
	"#00FFFF",
	"#FFFF00",
	"#000000",
	"#9BADFF"
]

class ValueMap {
	constructor() {
		this.values = new Uint8Array(4 * 4);
		this.sizeX = 4;
		this.sizeZ = 4;
		this.reset();
	}


	reset() {
		this.sizeX = 4;
		this.sizeZ = 4;
		this.values = new Uint8Array(this.sizeX * this.sizeZ);

		for (let x = 0; x < this.sizeX; x++)
		{
			for (let z = 0; z < this.sizeZ; z++)
			{
				this.values[x + this.sizeZ * z] = Math.floor(Math.random() * 8);
			}
		}
	}


	chooseRandomNumber() {
		let numArguments = arguments.length;
		return arguments[Math.floor(Math.random() * arguments.length)];
	}


	smooth() {
		let sizeZ = this.sizeZ - 2;
		let sizeX = this.sizeX - 2;
		let cache = new Uint8Array((this.sizeX - 2) * (this.sizeZ - 2));
		for (let z = 0; z < sizeZ; z++)
		{
			for (let x = 0; x < sizeX; x++)
			{
				let val   = this.values[x + 1 + (z + 1) * this.sizeX];
				let above = this.values[x + 1 + z       * this.sizeX];
				let below = this.values[x + 1 + (z + 2) * this.sizeX];
				let left  = this.values[x     + (z + 1) * this.sizeX];
				let right = this.values[x + 2 + (z + 1) * this.sizeX];

				if ((left == right) && (above == below))
				{
					if (Math.random() < 0.5)
					{
						val = left;
					}
					else
					{
						val = below;
					}
				}
				else
				{
					if (left == right)
					{
						val = left;
					}
					if (above == below)
					{
						val = above;
					}
				}
				cache[x + z * sizeX] = val;
			}
		}
		this.values = cache;
		this.sizeX -= 2;
		this.sizeZ -= 2;
	}


	zoom() {
		let lowStepX = (this.sizeX - 1) * 2;
		let lowStepZ = (this.sizeZ - 1) * 2;
		let cache = new Uint8Array(lowStepX * lowStepZ);
		for (let z = 0; z < this.sizeZ - 1; z++)
		{
			let idx = (z * 2) * lowStepX;
			let PrevZ0 = this.values[z * this.sizeX];
			let PrevZ1 = this.values[(z + 1) * this.sizeX];

			for (let x = 0; x < this.sizeX - 1; x++)
			{
				let ValX1Z0 = this.values[x + 1 + z * this.sizeX];
				let ValX1Z1 = this.values[x + 1 + (z + 1) * this.sizeX];
				cache[idx] = PrevZ0;
				cache[idx + lowStepX] = this.chooseRandomNumber(PrevZ0, PrevZ1);
				cache[idx + 1] = this.chooseRandomNumber(PrevZ0, ValX1Z0);
				cache[idx + 1 + lowStepX] = this.chooseRandomNumber(PrevZ0, ValX1Z0, PrevZ1, ValX1Z1);
				idx += 2;
				PrevZ0 = ValX1Z0;
				PrevZ1 = ValX1Z1;
			}
		}
		this.values = cache;
		this.sizeX = lowStepX;
		this.sizeZ = lowStepZ;
	}


	visualize(context, canvas) {
		context.clearRect(0, 0, canvas.width, canvas.height);
		const squareSizeX = canvas.width / (this.sizeX - 1) - g_DistanceBetweenSquares;
		const squareSizeY = canvas.height / (this.sizeZ - 1) - g_DistanceBetweenSquares;
		for (let x = 0; x < this.sizeX - 1; x++)
		{
			for (let y = 0; y < this.sizeZ - 1; y++)
			{
				let renderX = canvas.width / (this.sizeX - 1) * x + g_DistanceBetweenSquares;
				let renderY = canvas.height / (this.sizeZ - 1) * y + g_DistanceBetweenSquares;
				context.fillStyle = g_Colors[this.values[x + y * this.sizeZ]];
				context.fillRect(renderX, renderY, squareSizeX, squareSizeY);
			}
		}

		context.save();
		context.globalCompositeOperation = 'difference';
		context.fillStyle = 'white';
		context.fillText("Size: " + (this.sizeX - 1) + "x" + (this.sizeZ - 1), 5, 10);
		context.restore();
	}
}
