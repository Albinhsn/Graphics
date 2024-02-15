// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The transformation first applies scale, then rotation, and finally translation.
// The given rotation value is in degrees.
function GetTransform( positionX, positionY, rotation, scale )
{
  const degreesToRads = deg => (deg * Math.PI) / 180.0;
  const rot = degreesToRads(rotation);
  let scaleM = [
    scale, 0, 0,
    0, scale, 0,
    0, 0, 1 
  ];

  let rotationM = [
    Math.cos(rot), Math.sin(rot), 0,
    -Math.sin(rot), Math.cos(rot), 0,
    0,0,1
  ];

  let translationM = Array(
    1, 0, 0,
    0, 1, 0, 
    positionX, positionY, 1
  );
  let m = ApplyTransform(scaleM, rotationM);
  return ApplyTransform(m, translationM);
}

// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The arguments are transformation matrices in the same format.
// The returned transformation first applies trans1 and then trans2.
function ApplyTransform( trans1, trans2 )
{
  let M = [0,0,0,0,0,0,0,0,0];
  for(let i = 0; i < 3; i++){
    for(let j = 0; j < 3; j++){
      for(let k = 0; k < 3; k++){
        M[i + j * 3] += trans2[i + k * 3 ] * trans1[k + j * 3];
      }
    }
  }
  return M;
}
