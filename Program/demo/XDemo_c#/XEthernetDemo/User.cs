using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenCvSharp;
using Microsoft.ML.OnnxRuntime.Tensors;
using Microsoft.ML.OnnxRuntime;


namespace XEthernetDemo
{
    public class User
    {

        public static InferenceSession IS=null;
        private  string modelPath = @".\vec_class_x.onnx";
        public User( )
        {

            Console.WriteLine("start model");
            InitOnnx(modelPath);
            Console.WriteLine("end  model");
        }
        public static void InitOnnx(string modelPath)
        {
            IS = new InferenceSession(modelPath);
        }

        public static Prediction InferenceModel(Mat image)
        {
            Cv2.Resize(image, image, new OpenCvSharp.Size(64, 64), 1, 1, InterpolationFlags.Cubic);


            Tensor<float> input1 = new DenseTensor<float>(new[] { 1, 1, 64, 64 });

            for (int y = 0; y < image.Height; y++)
            {

                for (int x = 0; x < image.Width; x++)
                {

                    input1[0, 0, y, x] = (float)image.Get<byte>(y, x) / 255;

                }
            }

            // Setup inputs
            var input = new List<NamedOnnxValue>
            {
            NamedOnnxValue.CreateFromTensor("input.1", input1)
            };
            // Run inference
            // var session = new InferenceSession(modelFilePath);
            IDisposableReadOnlyCollection<DisposableNamedOnnxValue> results = IS.Run(input);

            // Postprocess to get softmax vector
            IEnumerable<float> output = results.First().AsEnumerable<float>();
            float sum = output.Sum(x => (float)Math.Exp(x));
            IEnumerable<float> softmax = output.Select(x => (float)Math.Exp(x) / sum);

            // Extract top 10 predicted classes
            IEnumerable<Prediction> top1 = softmax.Select((x, i) => new Prediction { Label = LabelMap.Labels[i], Confidence = x })
                               .OrderByDescending(x => x.Confidence)
                               .Take(4);

            Prediction p = new Prediction();
            foreach (var t in top1)
            {
                if(t.Label == "LV")
                    p = t;
            }
            return p;
        }
    }
}
