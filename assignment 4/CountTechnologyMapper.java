/*
import org.apache.hadoop.io.IntWritable;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;
import java.util.List;

public class CountTechnologyMapper extends Mapper<Object, Text, Text, IntWritable> {
	private static final int MISSING = 9999;
	private final static IntWritable one = new IntWritable(1);
	private final Text word = new Text();
	
	@Override
	public void map(Object key, Text value, Context context) throws IOException, InterruptedException{
		String line = value.toString();
		
		if (line.contains("Tools, Languages/ Technologies:")){
			String[] l1 = line.split("\"");
			
			String l2 = l1[0].split(":")[1];
			
			List<String> l3 = List.of(l2.split(", "));
			
			while (l3.iterator().hasNext()){
				word.set(l3.iterator().next());
				context.write(word,one );
			}
			
			
		}
		
		
	}
	
}
*/