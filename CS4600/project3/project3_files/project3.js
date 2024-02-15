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
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tv), gl.STATIC_DRAW);
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
    gl.uniform2f(this.p0, pt[0].getAttribute("cx"), pt[0].getAttribute("cy"));
    gl.uniform2f(this.p1, pt[1].getAttribute("cx"), pt[1].getAttribute("cy"));
    gl.uniform2f(this.p2, pt[2].getAttribute("cx"), pt[2].getAttribute("cy"));
    gl.uniform2f(this.p3, pt[3].getAttribute("cx"), pt[3].getAttribute("cy"));
	}
	draw()
	{
		gl.useProgram( this.prog );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );

    const lineSize = 4 * 12;

		gl.vertexAttribPointer( this.mvp , 4, gl.FLOAT, false, lineSize, 0);
		gl.enableVertexAttribArray( this.mvp );

		gl.vertexAttribPointer( this.p0, 2, gl.FLOAT, false, lineSize, 4 * 4);
		gl.enableVertexAttribArray( this.p0 );

		gl.vertexAttribPointer( this.p1, 2, gl.FLOAT, false, lineSize, 6 * 4);
		gl.enableVertexAttribArray( this.p1 );

		gl.vertexAttribPointer( this.p2, 2, gl.FLOAT, false, lineSize, 8 * 4);
		gl.enableVertexAttribArray( this.p2 );

		gl.vertexAttribPointer( this.p3, 2, gl.FLOAT, false, lineSize, 10 * 4);
		gl.enableVertexAttribArray( this.p3 );

		gl.drawArrays( gl.LINE_STRIP, 0, 4 );
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
    float t1 = 1.0-t;
    float t2 = t1 * t1;
    float t3 = t2 * t;
    
    vec2 v0 = t3 * p0;
    vec2 v1 = (t2 * t * 3.0) * p1;
    vec2 v2 = (3.0 * t1 * t * t) * p2;
    vec2 v3 = (t * t * t) * p3;
    vec2 pos = v0 + v1 + v2 + v3; 
    
		// [TO-DO] Replace the following with the proper vertex shader code
		gl_Position = vec4(pos.x,pos.y,0,1) * mvp;
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
