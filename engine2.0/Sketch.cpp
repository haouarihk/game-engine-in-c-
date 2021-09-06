void setup()
{
	CreateCanvas(640, 480);
}

void draw()
{
	background(255);

	fill(255, 0, 0);
	strokeWeight(10);
	rect(20, 20, 200);

	fill(0, 255, 0);
	circle(200, 200, 50);
}