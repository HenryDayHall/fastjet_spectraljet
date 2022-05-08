//  Spectral package
//  Henry Day-Hall (henrydayhall@protonmail.com)
//
//  Pre-alpha. Likely slow and possibly buggy.
//  Be sure you have the spectraljet python library installed, or this won't run.
//
//----------------------------------------------------------------------
// This file is part of FastJet contrib.
//
// It is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.
//
// It is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#include "SpectralPlugin.hh"

// also needed for piping
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <cstdio>  // also needed for piping
#include "math.h" 
#include <cmath>
#include <map>
#include <sstream>
#include <queue>

#define PI 3.14159265
#define RPARAM 1.0
#define LARGE_NUMBER 10e20

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib {

////////
//
//  Helper Functions and Types
//
////////
//----------------------------------------------------------------------

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


/// a function that pretty prints a list of jets
void print_jets (const fastjet::ClusterSequence & clust_seq, 
  		 const vector<fastjet::PseudoJet> & jets) {
  
    // sort jets into increasing pt
    vector<fastjet::PseudoJet> sorted_jets = sorted_by_pt(jets);  
  
    // label the columns
    printf("%5s %10s %10s %10s %10s %10s %10s\n","jet #", "rapidity", 
  	 "phi", "pt","m","e", "n constituents");
    
    // print out the details for each jet
    for (unsigned int i = 0; i < sorted_jets.size(); i++) {
      int n_constituents = clust_seq.constituents(sorted_jets[i]).size();
      printf("%5u %10.3f %10.3f %10.3f %10.3f %10.3f %8u\n",
  	   i, sorted_jets[i].rap(), sorted_jets[i].phi(),
  	   sorted_jets[i].perp(),sorted_jets[i].m(),sorted_jets[i].e(), n_constituents);
    }
}

////////
//
//  CoreJetAlgorithm
//
////////

void CoreJetAlgorithm::run_clustering(ClusterSequence & clust_seq) const {
    cout << "Getting kinematics\n";
    std::string kinematics = CoreJetAlgorithm::GetKinematicsString(clust_seq);
    add_inputs_to_converter(clust_seq);
    cout << "Trying a python call\n";
    // cal the python function
    std::string command = "python3 -m spectraljet.interface Spectral {} ";
    command.append(kinematics);
    string output = exec(command.c_str());
    process_output(output);


    int child1, child2, parent;
    for(auto cluster : cluster_labels){
        child1 = label_convertor[cluster[0]];
        child2 = label_convertor[cluster[1]];
        parent = cluster[2];
        int converted_parent;
        clust_seq.plugin_record_ij_recombination(child1, child2, 0., converted_parent);
        label_convertor[parent] = converted_parent;

    }

    for(int remaining : remaining_labels){
      int converted = label_convertor[remaining];
      clust_seq.plugin_record_iB_recombination(converted, 0.);
      }
}

string CoreJetAlgorithm::description () const {
  ostringstream desc;
  
  desc << "CoreJetAlgorithm base class";

  return desc.str();
}
	


void CoreJetAlgorithm::process_output(const std::string & output) const {
    std::string buf;            
    std::stringstream ss(output);

    //inputs
    ss >> buf;  // clear the input_labels
    while (ss >> buf){
        if(buf == "cluster_sequence"){
            break;
        }else if (buf != "[" && buf != "]"){
            input_labels.push_back(std::stoi(buf));
        }
	}


    // clusters
    int bracket_count = 0;
    int cluster_count = 0;
    int cluster_part = 0;

    while (ss >> buf){
        if(buf == "remaining"){
            break;
        }else if (buf == "["){
            bracket_count++;
            if (bracket_count == 2){
                cluster_count++;
                cluster_part = 0;
                cluster_labels.push_back(std::array<int, 3>());
            }
        }else if (buf == "]"){
            bracket_count--;
        }else{
            cluster_labels[cluster_count-1][cluster_part] = std::stoi(buf);
            cluster_part++;
        }
	}

    // remaining
    while (ss >> buf){
        if (buf != "[" && buf != "]"){
            remaining_labels.push_back(std::stoi(buf));
        }
   }
std::cout << std::endl;

}


std::string CoreJetAlgorithm::GetKinematicsString(const ClusterSequence & clust_seq) const {
    std::string kinematics = "";
    std::array<double, 7> kinematic_values;
    for(const auto jet : clust_seq.jets()){
        kinematic_values[0] = jet.e();
        kinematic_values[1] = jet.px();
        kinematic_values[2] = jet.py();
        kinematic_values[3] = jet.pz();
        kinematic_values[4] = jet.pt();
        kinematic_values[5] = jet.rapidity();
        kinematic_values[6] = jet.phi();
        for(double x : kinematic_values){
            kinematics += std::to_string(x) + " ";
        }
    }
    return kinematics;
}

void CoreJetAlgorithm::add_inputs_to_converter(const ClusterSequence & clust_seq) const {
    int i = 0;
    for(const auto jet : clust_seq.jets()){
        label_convertor[i] = i;
        i++;
    }
}


void CoreJetAlgorithm::PrintJet(const PseudoJet& jet) const {
  cout << "(E,pt,eta,phi,m)=(" << jet.e()<<", "<<jet.perp()<<", "<<jet.eta() <<", " << jet.m() <<", " << jet.phi()<<  ")" << endl;
}

SpectralLong::SpectralLong(double unneeded)
    :my_unneeded(unneeded)
{
}

string SpectralLong::description () const {
    ostringstream desc;

  desc << "SpectralLong";
  return desc.str();

}

}

FASTJET_END_NAMESPACE

