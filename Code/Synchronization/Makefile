JFLAGS = -g
JC = javac

.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = Coordinator.java \
		  Producer.java \
		  Consumer.java \
		  Buffer.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
	
run:
	java main
