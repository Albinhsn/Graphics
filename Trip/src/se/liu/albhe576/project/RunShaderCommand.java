package se.liu.albhe576.project;

public class RunShaderCommand implements Command{
    private final Shader shader;
    public RunShaderCommand(Shader shader){
        this.shader = shader;
    }

    @Override
    public void execute(GPUState state) {
        this.shader.execute(state.framebuffer);
    }
}
