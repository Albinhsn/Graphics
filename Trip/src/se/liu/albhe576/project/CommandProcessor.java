package se.liu.albhe576.project;

public class CommandProcessor implements Runnable
{
    private final RingBuffer<Command> commandQueue;
    private final GPUState state;
    private Thread graphicsLayerThread;

    public CommandProcessor(short imageWidth, short imageHeight){
        this.state = new GPUState(imageWidth, imageHeight);
        this.commandQueue = new RingBuffer<Command>(10);
    }


    private void processQueue(){
        Command c;
        while(this.graphicsLayerThread.isAlive()){
            c = commandQueue.deque();
            if(c != null){
                c.execute(this.state);
            }
        }
    }
    public void sendCommmand(Command command){
        this.commandQueue.enqueue(command);
    }

    @Override public void run() {
        this.graphicsLayerThread = new Thread(this.state.graphicsLayer);
        this.graphicsLayerThread.start();

        this.processQueue();
    }
}
