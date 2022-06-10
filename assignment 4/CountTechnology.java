// Implements Question 1 Assignment 4

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Job;

import java.io.IOException;
import java.util.List;

class CountTechnologyReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
	
	private final IntWritable result = new IntWritable();
	
	public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException{
		int sum=0;
		for(IntWritable val : values){
			sum+=val.get();
		}
		result.set(sum);
		context.write(key, result);
	}
	
}


class CountTechnologyMapper extends Mapper<Object, Text, Text, IntWritable> {
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


public class CountTechnology {
	public static void main(String args[]) throws IOException, InterruptedException, ClassNotFoundException {
		System.out.println("hello");
		
		if(args.length != 2){
			System.err.println("Usage: Count Technology <input path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		job.setJarByClass(CountTechnology.class);
		job.setJobName("Count Technology");
		
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		job.setMapperClass(CountTechnologyMapper.class);
		job.setCombinerClass(CountTechnologyReducer.class);
		job.setReducerClass(CountTechnologyReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		
		System.exit(job.waitForCompletion(true)? 0:1);
		
	}
}
