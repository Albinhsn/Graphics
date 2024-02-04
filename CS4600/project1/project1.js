// bgImg is the background image to be modified.
// fgImg is the foreground image.
// fgOpac is the opacity of the foreground image.
// fgPos is the position of the foreground image in pixels. It can be negative and (0,0) means the top-left pixels of the foreground and background are aligned.
function composite( bgImg, fgImg, fgOpac, fgPos )
{
  // Iterate from fgPos to fgPos + fgImg.width 
  for(let y = 0; y <  fgImg.height && y + fgPos.y < bgImg.height; y++){
    for (let x = 0; x < fgImg.width * 4  && x + (fgPos.x * 4) < bgImg.width * 4; x+= 4) {
      if(fgImg.data[y * fgImg.width * 4 + x + 3] != 0){
        for(let i = 0; i <= 3; i++){
          // alpha f * c f
          let fgBlend = fgOpac * fgImg.data[y * fgImg.width * 4 + x + i];
          // (1 - alpha f) * cb
          let bgBlend = (1-fgOpac) * bgImg.data[(y + fgPos.y) * bgImg.width  * 4 + (i + x + fgPos.x)];
          // c
          bgImg.data[y * bgImg.width * 4 + x + i] = fgBlend + bgBlend;
        }
      }
    }
  }
}
