package se.liu.albhe576.project;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Rasterizer
{
    private void drawLine(Image image, short x0, short y0, short x1, short y1, byte[] color){
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
            image.setPixel(steep ? y :x, steep ? x : y, color);
	    derror = (short) (derror + d);
            if(derror > dx){
                y = (short) (downwards ? y - 1 : y + 1);
		derror = (short) (derror - 2 * dx);
            }
        }

    }
    private short convertFromObjectSpaceToScreenSpace(float f, short dim){
        return (short)(((f + 1.0f) / 2.0f) * (dim - 1));
    }
    private Vector<Short> convertObjectSpaceVertexToScreenSpace(Vector<Float> v, short width, short height){
       return new Vector<>(
               convertFromObjectSpaceToScreenSpace(v.getX(), width),
               convertFromObjectSpaceToScreenSpace(v.getY(), height),
               convertFromObjectSpaceToScreenSpace(v.getZ(), width)
       );
    }

    public void rasterWavefront(Image image, Wavefront wavefront){
        short imageWidth  = image.getWidth();
        short imageHeight = image.getHeight();

        byte[][] colors = new byte[][]{
                new byte[]{(byte)255, (byte)255, (byte)255, (byte)255},
                new byte[]{(byte)255, (byte)255, (byte)0, (byte)255},
                new byte[]{(byte)255, (byte)0, (byte)255, (byte)255},
                new byte[]{(byte)0, (byte)255, (byte)255, (byte)255},
                new byte[]{(byte)255, (byte)0, (byte)0, (byte)255},
                new byte[]{(byte)0, (byte)0, (byte)255, (byte)255},
        };
        Random r = new Random();
        float [] zBuffer = new float[imageWidth * imageHeight];
        Arrays.fill(zBuffer, Float.MAX_VALUE);
        for(WavefrontFace[] face : wavefront.faces){
            Vector<Float> v0 = wavefront.vertices.get(face[0].vertexIdx - 1);
            Vector<Float> v1 = wavefront.vertices.get(face[1].vertexIdx - 1);
            Vector<Float> v2 = wavefront.vertices.get(face[2].vertexIdx - 1);

            Vector<Float> v2v0 = Vector.subtract(v2, v0);
            Vector<Float> v1v0 = Vector.subtract(v1, v0);
            Vector<Float> normal = Vector.cross(v2v0, v1v0);
            normal = Vector.normalize(normal);

            Vector<Float> lightDir = new Vector<>(0.0f,0.0f,-1.0f);

            float intensity = Vector.dot(normal, lightDir);

            byte[] color = new byte[]{
                    (byte)(255.0f * intensity),
                    (byte)(255.0f * intensity),
                    (byte)(255.0f * intensity),
                    (byte)255,
            };
            if(intensity > 0){
                this.raster(image, zBuffer, color, v0,v1,v2);
            }

        }
    }

    public void raster(Image image, float[] zBuffer, byte[] color, Vector<Float> v0, Vector<Float> v1, Vector<Float> v2){

        Vector<Short>v0Proj = this.convertObjectSpaceVertexToScreenSpace(v0, image.getWidth(), image.getHeight());
        Vector<Short> v1Proj = this.convertObjectSpaceVertexToScreenSpace(v1, image.getWidth(), image.getHeight());
        Vector<Short>v2Proj = this.convertObjectSpaceVertexToScreenSpace(v2, image.getWidth(), image.getHeight());

        short xMin = (short) Math.min(Math.min(v0Proj.getX(), v1Proj.getX()), v2Proj.getX());
        short xMax = (short) Math.max(Math.max(v0Proj.getX(), v1Proj.getX()), v2Proj.getX());

        short yMin = (short)Math.min(Math.min(v0Proj.getY(), v1Proj.getY()), v2Proj.getY());
        short yMax = (short)Math.max(Math.max(v0Proj.getY(), v1Proj.getY()), v2Proj.getY());

        float totalArea = Vector.crossProduct2D(v1Proj, v2Proj, v0Proj);

        for(short x = xMin; x <= xMax; x++){
            for(short y = yMin; y <= yMax; y++){
                Vector<Short> v = new Vector<>(x,y);
                float w0 = Vector.crossProduct2D(v1Proj, v2Proj, v);
                float w1 = Vector.crossProduct2D(v2Proj, v0Proj, v);
                float w2 = Vector.crossProduct2D(v0Proj, v1Proj, v);

                float alpha = w0 / totalArea;
                float beta  = w1 / totalArea;
                float gamma = w2 / totalArea;

                boolean inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

                short z = (short) (alpha * v0Proj.getZ() + beta * v1Proj.getZ() + gamma * v2Proj.getZ());

                if(inside){
                    zBuffer[y * image.getWidth() + x] = z;
                    image.setPixel(x,y,color);
                }

            }
        }
    }
}
