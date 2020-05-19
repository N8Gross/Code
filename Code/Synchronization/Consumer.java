import java.util.Formatter;

public class Consumer extends java.lang.Thread
{

	//Necessary variables and object declaration
    Buffer buf;
    Coordinator cdr;
	private int count;
	private static int checksum=0;
	private int current_thread;
	public int numEle=0; //initialize element counter

    // Consumer constructor
	public Consumer(Buffer buff, int count, int id) {
		this.buf = buff;
		this.count = count;
		this.current_thread = id;
	}

	// Method to output final checksum
    public static int getCheckSum(){
        return checksum;
    }

	public void consume(){
        synchronized(buf){
        //synchronized access to buffer
            try{
                while (buf.bufferEmpty()) {
                    buf.wait(); // wait if empty! (nothing to consume)
                }
                int index = buf.consReturnIndex(); //retrieve index number from buffer
                char element = buf.consumeBuffer(); //retrieve consumed character value
                numEle++; //+1 item consumed
                long time = cdr.getTime(); //retrieve current time
                System.out.printf("Consumer %3d consumed '%c' at index  %3d at %09dns\n",current_thread,element,index,time);
                checksum+=element; //add ASCII value to checksum
                buf.notify(); // notify waiting thread(s)!
            }catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            //run while the current thread is not interrupted
            //if number of elements consumed is less than the count, continue
            //else, interrupt
            if(numEle < count)
            {
                consume();
            }else{
                Thread.currentThread().interrupt();
            }
        }
    }
}


