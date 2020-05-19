public class Buffer{

	//Necessary variables and object declaration
	private int max_size; // max size of buffer based on user input
	private int front = 0; // index tracker for front of buffer (circular)
	private int back = 0; // index tracker for back of buffer (circular)
	private int[] buffer; // buffer array
	private int buffer_size = 0; // counter for current buffer size initialized to 0

    //Buffer constructor
	public Buffer(int size){
		this.max_size = size; // Max buffer size set at input size
		this.buffer = new int[max_size]; // Set buffer size to max_size passed
	}

	// Check if buffer is empty
	public boolean bufferEmpty() {
		if (buffer_size == 0) {
			return true; //its empty!
		}
		else {
			return false; //still filled
		}
	}

	// Check if buffer is full
	public boolean bufferFull() {
		if (buffer_size == max_size) {
			return true; //its filled!
		}
		else {
			return false; //its empty
		}
	}

	//Method to return index to Producer
	public int prodReturnIndex(){
        return back;
	}

	//Method to return index to Consumer
	public int consReturnIndex(){
        return front;
	}

	public void produceBuffer(int element)
	{
        buffer_size = buffer_size + 1; //update buffer size (number of elements in buffer)
        back = (back + 1) % max_size; //calculate updated index position in circular buffer
        buffer[back] = element; //insert element to back of circular buffer
    }

    public char consumeBuffer(){
        buffer_size = buffer_size - 1; //update buffer size (number of elements in buffer)
		front = (front + 1) % max_size; //calculate updated index position in circular buffer
        return (char)buffer[front]; //consume element to front of circular buffer
    }
}
