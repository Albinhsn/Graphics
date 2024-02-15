// [TO-DO] Complete the implementation of the following class and the vertex shader below.

class CurveDrawer {
	constructor()
	{
		this.prog   = InitShaderProgram( curvesVS, curvesFS );

    gl.useProgram(this.prog);

    this.mvp = gl.getUniformLocation(this.prog, "mvp");
    this.p0 = gl.getUniformLocation(this.prog, "p0");
    this.p1 = gl.getUniformLocation(this.prog, "p1");
    this.p2 = gl.getUniformLocation(this.prog, "p2");
    this.p3 = gl.getUniformLocation(this.prog, "p3");

    this.buffer = gl.createBuffer();
		
		// Initialize the attribute buffer
		this.steps = 100;
		var tv = [];
		for ( var i=0; i<this.steps; ++i ) {
			tv.push( i / (this.steps-1) );
		}
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(p), gl.STATIC_DRAW);
	}
	setViewport( width, height )
	{
		gl.useProgram( this.prog );	// Bind the program
    const trans = [2/width,0,0,0,  0,-2/height,0,0, 0,0,1,0,  -1,1,0,1];

    gl.uniformMatrix4fv(this.mvp, false, trans); 
	}
	updatePoints( pt )
	{
		gl.useProgram( this.prog );	// Bind the program
    gl.uniform2fv(this.p0, pt[0]);
    gl.uniform2fv(this.p1, pt[1]);
    gl.uniform2fv(this.p2, pt[2]);
    gl.uniform2fv(this.p3, pt[3]);
	}
	draw()
	{
		gl.useProgram( this.prog );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.vertPos, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.vertPos );
		gl.drawArrays( gl.LINE_STRIP, 0, 4 );
    console.log("Drawing!");
	}
}

// Vertex Shader
var curvesVS = `
	attribute float t;
	uniform mat4 mvp;
	uniform vec2 p0;
	uniform vec2 p1;
	uniform vec2 p2;
	uniform vec2 p3;
	void main()
	{
		// [TO-DO] Replace the following with the proper vertex shader code
		gl_Position = vec4(p0.x,p0.y,0,1);
	}
`;

// Fragment Shader
var curvesFS = `
	precision mediump float;
	void main()
	{
		gl_FragColor = vec4(1,0,0,1);
	}
`;
