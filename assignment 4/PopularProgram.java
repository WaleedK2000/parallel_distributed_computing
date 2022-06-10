// Implements Question 2a Assignment 4


import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;
import java.util.Objects;

class PopularProgramMapperSecond extends Mapper<Object, Text, Text, Text> {
	private static final int MISSING = 9999;
	private final static Text val = new Text();
	private final Text word = new Text();
	
	@Override
	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		String line = value.toString();
		
		
		
		String[] vals = line.split(" ");
		String campuses = vals[0];
		String course_count =  vals[1] + "," + vals[2];
		
		word.set(campuses);
		val.set(course_count);
		context.write(word,val);
		
	
	}
	
}

class PopularProgramReducerSecond extends Reducer<Text, Text, Text, IntWritable> {
	
	private final IntWritable result = new IntWritable();
	
	@Override
	public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException{
		
		String course = "";
		int count = 0;
		
		for(Text val : values) {
			String[] value = val.toString().split(",");
			int current = Integer.parseInt(value[1]);
			
			if(current>count){
				course = value[0];
			}
		}
		
		result.set(count);
		context.write(key, result);
	}
	
}

class PopularProgramCountReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
	
	private final IntWritable result = new IntWritable();
	@Override
	public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException{
		
		int sum=0;
		for(IntWritable val : values){
			sum+=val.get();
		}
		result.set(sum);
		context.write(key, result);
	}
	
}

class PopularProgramCountMapper extends Mapper<Object, Text, Text, IntWritable> {
	private static final int MISSING = 9999;
	private final static IntWritable one = new IntWritable(1);
	private final Text word = new Text();
	
	@Override
	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		String line = value.toString();
		
		String[] vals = line.split(",");
		String[] campuses = {vals[4], vals[5], vals[6], vals[7]};
		String[] courses = {vals[8], vals[9], vals[10], vals[11]};
		
		for (int i = 0; i < 4; ++i) {
			if (Objects.equals(campuses[i], "")) {
				break;
			} else {
				
				for (int j = 0; j < 4; ++j) {
					if (Objects.equals(courses[j], "")) {
						break;
					} else {
						String campusCourse = campuses[i]+ "," + courses[j];
						word.set(campusCourse);
						context.write(word,one);
					}
				}
				
			}
		}
	}
}

public class PopularProgram {
	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
		System.out.println("hello");
		
		if (args.length != 2) {
			System.err.println("Usage: Count Technology <input path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		Job job2 = new Job();
		
		job.setJarByClass(PopularProgram.class);
		
		job2.setJarByClass(PopularProgram.class);
		job.setJobName("Count Technology");
		job2.setJobName("Reduce to Ans");
		
		
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		FileInputFormat.addInputPath(job2, new Path(args[1]));
		FileOutputFormat.setOutputPath(job2, new Path(args[1] + "_job2"));
		
		job.setMapperClass(PopularProgramCountMapper.class);
		job.setCombinerClass(PopularProgramCountReducer.class);
		job.setReducerClass(PopularProgramCountReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		
		
		
		job2.setMapperClass(PopularProgramMapperSecond.class);
		job2.setCombinerClass(PopularProgramReducerSecond.class);
		job2.setReducerClass(PopularProgramReducerSecond.class);
		job2.setOutputKeyClass(Text.class);
		job2.setOutputValueClass(IntWritable.class);
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
		
	}
}
