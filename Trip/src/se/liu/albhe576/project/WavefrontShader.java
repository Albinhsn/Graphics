package se.liu.albhe576.project;

import java.awt.*;
import java.util.Arrays;

public class WavefrontShader extends Shader {
    Wavefront wavefront;
    private final Vector<Float> lightDir = new Vector<>(0.0f, 0.0f, -1.0f);
    public WavefrontShader(Wavefront wavefront){
        this.wavefront = wavefront;
    }
    @Override
    public void execute(Framebuffer framebuffer) {
        Vertex[] vertices = this.vertex();
        this.rasterize(vertices, framebuffer);
    }

    @Override
    protected Vertex[] vertex() {
        int size = this.wavefront.faces.size();
        Vertex[] vertices = new Vertex[size];
        for(int i = 0; i < size; i++){
            WavefrontFace[] face = this.wavefront.faces.get(i);
            Vertex vert = new Vertex();

            Vector<Float> v0 = wavefront.vertices.get(face[0].vertexIdx - 1);
            Vector<Float> v1 = wavefront.vertices.get(face[1].vertexIdx - 1);
            Vector<Float> v2 = wavefront.vertices.get(face[2].vertexIdx - 1);

            Vector<Float> v2v0 = Vector.subtract(v2, v0);
            Vector<Float> v1v0 = Vector.subtract(v1, v0);
            Vector<Float> normal = Vector.cross(v2v0, v1v0);
            vert.normal = Vector.normalize(normal);

            vert.position[0] = v0;
            vert.position[1] = v1;
            vert.position[2] = v2;

            vertices[i] = vert;

        }
        return vertices;
    }

    protected void rasterize(Vertex[] vertices, Framebuffer framebuffer){
        short imageWidth = framebuffer.getWidth();
        short imageHeight = framebuffer.getHeight();
        float[] zBuffer = new float[imageWidth * imageHeight];


        Arrays.fill(zBuffer, -Float.MAX_VALUE);
        for(Vertex vertex : vertices){
            Vector<Float> v0 = vertex.position[0];
            Vector<Float> v1 = vertex.position[1];
            Vector<Float> v2 = vertex.position[2];

            Vector<Short>v0Proj = this.convertObjectSpaceVertexToScreenSpace(v0, imageWidth, imageHeight);
            Vector<Short> v1Proj = this.convertObjectSpaceVertexToScreenSpace(v1, imageWidth, imageHeight);
            Vector<Short>v2Proj = this.convertObjectSpaceVertexToScreenSpace(v2, imageWidth, imageHeight);

            short xMin = (short) Math.min(Math.min(v0Proj.getX(), v1Proj.getX()), v2Proj.getX());
            short xMax = (short) Math.max(Math.max(v0Proj.getX(), v1Proj.getX()), v2Proj.getX());

            short yMin = (short)Math.min(Math.min(v0Proj.getY(), v1Proj.getY()), v2Proj.getY());
            short yMax = (short)Math.max(Math.max(v0Proj.getY(), v1Proj.getY()), v2Proj.getY());

            float totalArea = Vector.crossProduct2D(v1Proj, v2Proj, v0Proj);

            byte[] color = new byte[4];

            for(short x = xMin; x <= xMax; x++){
                for(short y = yMin; y <= yMax; y++){
                    Vector<Short> v = new Vector<>(x,y);
                    float w0 = Vector.crossProduct2D(v1Proj, v2Proj, v);
                    float w1 = Vector.crossProduct2D(v2Proj, v0Proj, v);
                    float w2 = Vector.crossProduct2D(v0Proj, v1Proj, v);

                    float alpha = w0 / totalArea;
                    float beta  = w1 / totalArea;
                    float gamma = w2 / totalArea;

                    Vector<Float> bary = new Vector<>(alpha, beta, gamma);

                    boolean inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

                    float z = alpha * v0.getZ() + beta * v1.getZ() + gamma * v2.getZ();

                    if(inside && zBuffer[y * framebuffer.getWidth() + x] < z && this.fragment(bary, vertex.normal, color)){
                        zBuffer[y * framebuffer.getWidth() + x] = z;
                        framebuffer.setPixel(x,y,color);
                    }

                }
            }

        }
    }

    protected boolean fragment(Vector<Float> barycentric, Vector<Float> normal, byte[] color) {
        float intensity = Vector.dot(normal, lightDir);
        if(intensity < 0){
            return false;
        }

        color[0] = (byte)(255.0f * intensity);
        color[1] = (byte)(255.0f * intensity);
        color[2] = (byte)(255.0f * intensity);
        color[3] = (byte)255;

        return true;
    }
}
