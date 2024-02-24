package se.liu.albhe576.project;

public class WavefrontFace
{
    public int vertexIdx;
    public int textureIdx;
    public int normalIdx;
    public WavefrontFace(int vIdx, int tIdx, int nIdx){
        this.vertexIdx = vIdx;
        this.textureIdx= tIdx;
        this.normalIdx = nIdx;
    }
    public WavefrontFace(int []indices){
        this.vertexIdx = indices[0];
        this.textureIdx= indices[1];
        this.normalIdx = indices[2];
    }
}
