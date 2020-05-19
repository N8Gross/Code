import java.util.Random;
import java.util.Formatter;
//import static Coordinator.getTime;

public class Producer extends java.lang.Thread
{

	//Necessary variables and object declaration
	Random randomWithSeed;
	Coordinator cdr;
	Buffer buf;
	private int current_thread;
	private int seed;
	private int count;
	private int numEle=0; //initialize element counter
	public static int checksum=0; //initialize checksum

	//Producer constructor
    public Producer(Buffer buff, int count, int id, int seed){
		//Assign values to the variables
		randomWithSeed = new Random(seed+id);
		this.current_thread=id;
		this.buf=buff;
		this.count=count;
		this.seed=seed;
	}

    // Method to output final checksum
    public static int getCheckSum(){
        return checksum;
    }

    public void produce(char back_element) {
        synchronized(buf){
        //synchronized access to buffer
            try{
                while (buf.bufferFull()) {
                    buf.wait(); // wait if full! (can't produce any more)
                }
                int index = buf.prodReturnIndex(); //retrieve index number from buffer
                buf.produceBuffer(back_element); //send in Randum character generated into back of buffer
                numEle++; //+1 item produced
                long time = cdr.getTime(); //retrieve current time
                System.out.printf("Producer %3d inserted '%c' at index  %3d at %09dns\n",current_thread,back_element,index,time);
                checksum+=back_element; //add ASCII value to checksum
                buf.notify(); // notify waiting thread(s)!
            }catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

	@Override
	public void run()
	{
		while (!Thread.currentThread().isInterrupted()) {
            if(numEle < count){
                // Get random character and produce into buffer if thread is not interrupted
                char item = (char) ('A' + randomWithSeed.nextInt(26));
                produce(item); //send to produce method to insert into buffer
            }else{
                Thread.currentThread().interrupt();
            }
        }
	}
}

