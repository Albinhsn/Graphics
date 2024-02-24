package se.liu.albhe576.project;

public class CommandProcessor implements Runnable
{
    RingBuffer<Command> commandQueue;

    public void execute(Command c){

    }

    public CommandProcessor(){
	this.commandQueue = new RingBuffer<Command>(10);
    }


    public void processQueue(){
	Command c;
	while(true){
	    c = commandQueue.deque();
	    if(c != null){
		this.execute(c);
	    }

	}
    }

    @Override public void run() {

    }
}
