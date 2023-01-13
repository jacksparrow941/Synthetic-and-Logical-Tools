#include <mockturtle/mockturtle.hpp>
#include<mockturtle/algorithms/collapse_mapped.hpp>
#include<mockturtle/views/depth_view.hpp>
#include<mockturtle/views/mffc_view.hpp>
#include <mockturtle/networks/klut.hpp>
#include <lorina/aiger.hpp>
#include <lorina/verilog_regex.hpp>
#include<mockturtle/algorithms/cut_rewriting.hpp>
#include<mockturtle/algorithms/lut_mapping.hpp>
#include<mockturtle/views/mapping_view.hpp>

using namespace mockturtle;
using namespace percy;
using namespace std;

int main()
{
  aig_network aig;
 //auto const result = lorina::read_verilog( "../experiments/benchmarks/rca2.v", verilog_reader(aig));
//  assert( result == lorina::return_code::success );

  string vis[20];
   
    vis[0]="../experiments/benchmarks/ethernet.aig";
         vis[1]="../experiments/benchmarks/hyp.aig";
         
         //vis[2]="../experiments/benchmarks/max.aig";
         
      /*   vis[0]="../experiments/benchmarks/prime4.aig";
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
         */
         
    
  for(int i=0;i<2;i++)
  {
     char name[200]={}; 
    for(int j=26;j<vis[i].length();j++)
    {  name[j-26]=vis[i][j];
    }
   
   
   
   
   cout<<"FILE NAME :  ";
  
     cout<<name;
     
     cout<<"\n";
   
   auto const result=lorina::read_aiger( vis[i],aiger_reader(aig));
  assert( result == lorina::return_code::success );
  
  
  depth_view aig_depth{aig};
  std::cout << "Depth: " << aig_depth.depth() << "\n";


   
   
 	cut_rewriting_params pss;
  	pss.cut_enumeration_ps.cut_size = 3;
 
   	uint32_t size_before = aig.num_gates();
   
 
   
   
   exact_aig_resynthesis resyn;
   std::cout<<"Before num of gates  "<<aig.num_gates()<<"\n";
   cut_rewriting(aig,resyn,pss);
   aig=cleanup_dangling(aig);
   


    std::cout<<"After cut rewriting  num of gates  "<<aig.num_gates()<<"\n";
       
    lut_mapping_params ps;
    lut_mapping_stats st;
    ps.cut_enumeration_ps.cut_size = 3;
    mapping_view<aig_network, true> mapped_aig{aig};
    lut_mapping<decltype( mapped_aig ), true>( mapped_aig, ps ,&st);
    
    const auto klut = *collapse_mapped_network<klut_network>( mapped_aig );
  

     uint32_t size_before1 = klut.num_gates();
     
     std::cout<<"After  LUT mapping  "<<size_before1<<"\n";
     depth_view aig_depth12{klut};
     std::cout << "Depth: "<< aig_depth12.depth() << "\n"<<"\n";
     
  

   }
   
   
  return 0;
 
 
}
