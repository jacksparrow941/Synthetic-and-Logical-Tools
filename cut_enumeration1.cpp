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
#include<iostream>
#include<cstring>
using namespace std;
using namespace mockturtle;
using namespace percy;

int main()
{
  mig_network aig;

 //auto const result = lorina::read_verilog( "../experiments/benchmarks/rca2.v", verilog_reader(aig));
//  assert( result == lorina::return_code::success );

  string vis[13];
  
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
	cout<<name<<"\n";
   
  auto const result=lorina::read_aiger( vis[i],aiger_reader(aig));
   //auto const result=lorina::read_aiger( "../experiments/benchmarks/prime4.aig",aiger_reader(aig));
  assert( result == lorina::return_code::success );
  
  
  depth_view mig_depth{aig};



   uint32_t size_before = aig.num_gates();

   std::cout << "Depth: " << mig_depth.depth() << "\n";
   std::cout<<"Before num of gates  "<<size_before<<"\n";
 


/* node resynthesis */
   mig_npn_resynthesis resyn;
   cut_rewriting_params ps;
   ps.cut_enumeration_ps.cut_size = 4;
   aig = cut_rewriting( aig, resyn, ps );
   aig = cleanup_dangling( aig );
 

   
    uint32_t size_before2 = aig.num_gates();
     
     std::cout<<size_before2<<"\n";
     
     mapping_view<mig_network, true> mapped_mig{aig};

    lut_mapping_params ps2;
    lut_mapping_stats st;
    ps2.cut_enumeration_ps.cut_size = 3;
     lut_mapping<decltype( mapped_mig ), true>( mapped_mig, ps2 ,&st);
     const auto klut = *collapse_mapped_network<klut_network>( mapped_mig );
     
     uint32_t size_before1 = klut.num_gates();
     
     std::cout<<"After LUT mapping in M "<<size_before1<<"\n";
     depth_view mig_depth12{klut};
     std::cout << "Depth: "<< mig_depth12.depth() << "\n" <<"\n";
     
     
    } 
    


   
  return 0;
 
 
}

