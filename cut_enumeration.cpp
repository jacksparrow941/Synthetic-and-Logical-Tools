#include <mockturtle/algorithms/mig_resub.hpp>
#include <mockturtle/algorithms/cleanup.hpp>
#include <mockturtle/io/aiger_reader.hpp>
#include <mockturtle/networks/mig.hpp>
#include <mockturtle/views/depth_view.hpp>
#include <mockturtle/views/fanout_view.hpp>

using namespace mockturtle;
using namespace std;
int main()
{
  mig_network mig;
   string vis[10];
         vis[0]="../experiments/benchmarks/prime4.aig";
         vis[1]="../experiments/benchmarks/adder.aig";
         vis[2]="../experiments/benchmarks/max.aig";
         vis[3]="../experiments/benchmarks/sin.aig";        
          vis[4]="../experiments/benchmarks/usb_phy.aig";
         vis[5]="../experiments/benchmarks/sasc.aig";        
          vis[6]="../experiments/benchmarks/cavlc.aig";
         vis[7]="../experiments/benchmarks/simple_spi.aig";         
         vis[8]="../experiments/benchmarks/i2c.aig";
         vis[9] ="../experiments/benchmarks/RISC.aig";
         vis[10] ="../experiments/benchmarks/DSP.aig";
         vis[11] ="../experiments/benchmarks/div.aig";
         vis[12] ="../experiments/benchmarks/mem_ctrl.aig";
         
  for(int i=0;i<13;i++)
  {

   auto const result=lorina::read_aiger( vis[i],aiger_reader(mig));
  assert( result == lorina::return_code::success );
    depth_view depth_mig1{mig};
 
   //lorina::read_verilog( "../experiments/benchmarks/adder.v", mockturtle::verilog_reader( aig ) );
  //assert( result == lorina::return_code::success );
  
   std::cout<<"Before rebustitution num of gates "<<mig.num_gates()<<"\n";
   std::cout<<"Depth : "<<depth_mig1.depth()<<"\n";
   resubstitution_params ps;
    resubstitution_stats st;
    ps.max_pis = 8u;
    ps.max_inserts = 1u;
    ps.progress = false;

   
    
    depth_view depth_mig{mig};
     fanout_view fanout_mig{depth_mig};
     
    uint32_t const size_before = fanout_mig.num_gates();
    mig_resubstitution( fanout_mig, ps, &st );
    mig = cleanup_dangling( mig );
    
     depth_view depth_mig2{mig};
      fanout_view fanout_mig1{depth_mig2};
     
    uint32_t const size_after = fanout_mig1.num_gates();
    
    std::cout<<"After rebustitution num of gates "<<mig.num_gates()<<"\n";
    std::cout<<"Depth : "<<depth_mig2.depth()<<"\n"<<"\n";
     std::cout<<"Size : "<<size_after<<"\n"<<"\n";
    
   
   
   }
  return 0;
 
 
}

