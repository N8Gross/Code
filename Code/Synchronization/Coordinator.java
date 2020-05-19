import java.lang.Exception;
import java.util.Random;
import java.time.Instant;
import java.time.Clock;
import java.time.Duration;

class Coordinator
{

	public static void main(String[] args)
	{
        // Create necessary variables and import user inputs for evaluation
        int bufSize = Integer.parseInt(args[0]); //buffer size
        int numofElem = Integer.parseInt(args[1]); //number of elements
        int numofProd = Integer.parseInt(args[2]); //number of producers
        int numofCons = Integer.parseInt(args[3]); //number of consumers
        int seed = Integer.parseInt(args[4]); //seed value
        int prodCount = numofElem/numofProd; //average number of elements per producer
        int consCount = numofElem/numofCons; //average number of elements per consumer

		 // Create buffer
		Buffer buf = new Buffer(bufSize);

		//Create thread arrays
        Thread[] producer = new Thread[numofProd];
        Thread[] consumer = new Thread[numofCons];

		for (int i = 1; i <= numofProd; i++){
            //Producer thread
            producer[i-1] = new Producer(buf,prodCount,i,seed);
            producer[i-1].start();
            }
        for (int i = 1; i <= numofCons; i++){
            //Consumer thread
            consumer[i-1] = new Consumer(buf,consCount,i);
            consumer[i-1].start();
            }

        //join the producer and consumer threads back together for final output
        try{
            for (int i = 1; i <= numofProd; i++){
                producer[i-1].join();
            }
            for (int i = 1; i <= numofCons; i++){
                consumer[i-1].join();
            }
        }catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        //print final checksum
        System.out.println("Produced " + numofElem + " items with checksum " + Producer.getCheckSum());
        System.out.println("Consumed " + numofElem + " items with checksum " + Consumer.getCheckSum());
	}

	// Save starting time
	private static final long start_time = System.nanoTime();

	//Call this function from your producer or your conusmer to get the time stamp to be displayed
	//package getTime;
	public static long getTime()
	{
		return System.nanoTime() - start_time;
	}
}
