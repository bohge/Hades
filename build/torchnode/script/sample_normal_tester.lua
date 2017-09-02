local distributions = require "distributions"
gnuplot = require "gnuplot"
require "arclight"

local function tester(count, mu, sigma)
  local nrd = torch.Tensor(count):zero();
  local nrdh = torch.Tensor(count):zero();
  local mvn = torch.Tensor(count):zero();
  for i=1, count do
    nrd[i] = arclight.sample_normal_distribution(mu, sigma);
    nrdh[i] = arclight.sample_normal_distribution(mu, sigma*0.5);
    --mvn[i] = distributions.mvn.rnd(torch.Tensor({mu}), torch.Tensor({sigma}));
  end  
  
  gnuplot.figure(1);
  gnuplot.title("my sample");
  gnuplot.axis({-100,100,0,count/2});
  gnuplot.hist(nrd);
  
  gnuplot.figure(2);
  gnuplot.title("half sample");
  gnuplot.axis({-100,100,0,count/2});
  gnuplot.hist(nrdh);
  
  --[[gnuplot.figure(2);
  gnuplot.title("mvn sample");
  gnuplot.hist(mvn);]]--
end


return tester;