package se.liu.albhe576.project;

import java.awt.*;
import java.util.Arrays;

public abstract class Shader
{
    protected static class Vertex{
        Vector<Float>[] position;
        Vector<Float> normal;

        public Vertex(){
            this.position = new Vector[3];
            this.normal = new Vector<>(3);
        }
    }
    public abstract void execute(Framebuffer framebuffer);

    protected abstract Vertex[] vertex();

    private void drawLine(Framebuffer framebuffer, short x0, short y0, short x1, short y1, byte[] color){
        boolean steep = Math.abs(x0 - x1) < Math.abs(y0 - y1);

        if(steep){
            short tmp = x0;
            x0 = y0;
            y0 = tmp;

            tmp = x1;
            x1 = y1;
            y1 = tmp;
        }

        if(x0 > x1){
            short tmp = x0;
            x0 = x1;
            x1 = tmp;

            tmp = y0;
            y0 = y1;
            y1 = tmp;
        }

        short dx = (short) (x1 - x0);
        short dy = (short) (y1 - y0);

        short d = (short) (2 * dy);
        boolean downwards = d < 0;
        d = downwards ? (short) -d : d;

        short derror = 0;
        short y = y0;
        for(short x = x0; x <= x1; x++){
            framebuffer.setPixel(steep ? y :x, steep ? x : y, color);
            derror = (short) (derror + d);
            if(derror > dx){
                y = (short) (downwards ? y - 1 : y + 1);
                derror = (short) (derror - 2 * dx);
            }
        }

    }
    protected short convertFromObjectSpaceToScreenSpace(float f, short dim){
        return (short)(((f + 1.0f) / 2.0f) * (dim - 1));
    }
    protected Vector<Short> convertObjectSpaceVertexToScreenSpace(Vector<Float> v, short width, short height){
        return new Vector<>(
                convertFromObjectSpaceToScreenSpace(v.getX(), width),
                convertFromObjectSpaceToScreenSpace(v.getY(), height),
                convertFromObjectSpaceToScreenSpace(v.getZ(), width)
        );
    }
}
