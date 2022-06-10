// Implements Question 2b Assignment 4

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

class IntermidiateRegionMapper extends Mapper<Object, Text, Text, IntWritable> {
	private static final int MISSING = 9999;
	private final static IntWritable one = new IntWritable(1);
	private final Text word = new Text();
	
	@Override
	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		String line = value.toString();
		
		String[] vals = line.split(",");
		String board =  vals[16];
		
		String nuTestBBA =  vals[23];
		String nuTestBS = vals[24];
		
		String sat_1_total = vals[25];
		String sat_1_obtained = vals[26];
		
		String sat_2_total = vals[27];
		String sat_2_obtained = vals[28];
		
		int count = 0;
		int adder = 0;
		if (nuTestBBA != ""){
			adder += Integer.parseInt(nuTestBBA);
			count++;
		}
		
		if (nuTestBS != ""){
			adder += Integer.parseInt(nuTestBS);
			count++;
		}
		
		if (sat_1_obtained != ""){
			adder += Integer.parseInt(sat_1_obtained) / Integer.parseInt(sat_1_total);
			count++;
		}
		
		if (sat_1_obtained != ""){
			adder += Integer.parseInt(sat_2_obtained) / Integer.parseInt(sat_2_total);
			count++;
		}
		
		if(count > 1){
			int avg = adder/count;
			
			word.set(board);
			one.set(count);
			
			context.write(word,one);
		}
	}
}

class IntermidiateRegionReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
	
	private final IntWritable result = new IntWritable();
	@Override
	public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException{
		
		int sum=0;
		int count = 0;
		for(IntWritable val : values){
			sum+=val.get();
			count++;
		}
		result.set(sum/count);
		context.write(key, result);
	}
	
}

public class IntermidiateRegion {
	
	public static void main(String args[]) throws IOException, InterruptedException, ClassNotFoundException {
		System.out.println("hello");
		
		if(args.length != 2){
			System.err.println("Usage: Count Technology <input path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		job.setJarByClass(IntermidiateRegion.class);
		job.setJobName("Compare Boards");
		
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		job.setMapperClass(IntermidiateRegionMapper.class);
		job.setCombinerClass(IntermidiateRegionReducer.class);
		job.setReducerClass(IntermidiateRegionReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		
		System.exit(job.waitForCompletion(true)? 0:1);
		
	}
	
}
