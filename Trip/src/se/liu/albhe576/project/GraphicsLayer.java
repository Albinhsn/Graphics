package se.liu.albhe576.project;


import org.lwjgl.BufferUtils;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;

import java.awt.image.Raster;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.system.MemoryStack.stackPush;
import static org.lwjgl.system.MemoryUtil.NULL;
public class GraphicsLayer implements Runnable
{
    private long window;
    private Image image;

    private boolean drawCallMade;

    private void draw(Image image){
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getBuffer());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(this.window);
    }


    private int createAndCompileShader(int shaderType, String shaderSourceLocation){
        int shaderId = glCreateShader(shaderType);

        String shaderSource;
        try{
            shaderSource = Files.readString(Path.of(shaderSourceLocation));
        }catch(IOException e){
            e.printStackTrace();
            return -1;
        }
        glShaderSource(shaderId, shaderSource);

        glCompileShader(shaderId);

        IntBuffer intBuffer = BufferUtils.createIntBuffer(1);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, intBuffer);
        if(intBuffer.get(0) != 1){
            String log = glGetShaderInfoLog(shaderId);
            System.out.println("Error loading shader\n");
            System.out.println(log);
            System.exit(2);
        }
 

        return shaderId;
    }

    private void createTextureShaderProgram(){

        final int vShader = createAndCompileShader(GL_VERTEX_SHADER, "./src/se/liu/albhe576/project/texture.vs");
        final int fShader = createAndCompileShader(GL_FRAGMENT_SHADER, "./src/se/liu/albhe576/project/texture.ps");


        final int programId = glCreateProgram();
        glAttachShader(programId, vShader);
        glAttachShader(programId, fShader);

        glBindAttribLocation(programId, 0, "inputPosition");
        glBindAttribLocation(programId, 1, "inputTexCoord");

        glLinkProgram(programId);
        glUseProgram(programId);

        float []bufferData  = new float[]{
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };

        int []indices = new int[]{0, 1, 2, 1, 3, 2};

        final int vertexArrayId = glGenVertexArrays();
        glBindVertexArray(vertexArrayId);

        final int vertexBufferId = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER,  bufferData, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, 12);
 

        final int indexBufferId = glGenBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);
    }

    // ToDo make this a ringbuffer of calls instead
    public void sendDrawCall(Image image){
        this.image = image;
        this.drawCallMade = true;
    }

    public void run(){
        this.initGLFW();
        this.createTextureShaderProgram();

        while(!glfwWindowShouldClose(window)){
            if(this.drawCallMade){
                this.drawCallMade = false;
                this.draw(this.image);
            }
            glfwPollEvents();
        }
    }


    private void initGLFW(){
        GLFWErrorCallback.createPrint(System.err).set();

        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // ToDo, obvisouly change this
        this.window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
        if(this.window == NULL){
            throw new RuntimeException("Failed to create the GLFW window");
        }

        glfwSetKeyCallback(this.window, (window, key, scancode, action, mods) -> {
            if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
                glfwSetWindowShouldClose(window, true);
            }

        });

        try(MemoryStack stack = stackPush()){
            IntBuffer pWidth = stack.mallocInt(1);
            IntBuffer pHeight = stack.mallocInt(1);

            glfwGetWindowSize(window, pWidth, pHeight);

            GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwSetWindowPos(window,
                             (vidmode.width() - pWidth.get(0)) / 2,
                             (vidmode.height() - pHeight.get(0)) / 2
                             );
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glfwShowWindow(window);

        GL.createCapabilities();
    }

    public GraphicsLayer(){
        this.image = null;
        this.drawCallMade = false;
    }

    public static void main(String[] args) {
    }
}
