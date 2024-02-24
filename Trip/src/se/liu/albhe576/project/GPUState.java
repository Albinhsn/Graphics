package se.liu.albhe576.project;

public class GPUState {
    public final GraphicsLayer graphicsLayer;
    public final Framebuffer framebuffer;

    public GPUState(short imageWidth, short imageHeight){
        this.framebuffer = new Framebuffer(imageWidth, imageHeight);
        this.graphicsLayer = new GraphicsLayer();
    }
}
