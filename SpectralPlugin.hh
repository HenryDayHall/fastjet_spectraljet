//  VariableR Package
//  Questions/Comments?  jthaler@jthaler.net
//
//  Copyright (c) 2009-2013
//  David Krohn, Jesse Thaler, and Lian-Tao Wang
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

#ifndef __FASTJET_CONTRIB_VARIABLER_HH__
#define __FASTJET_CONTRIB_VARIABLER_HH__

#include <fastjet/internal/base.hh>

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include <map>
#include <queue>

using namespace std;


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib {

////////
//
//  Helper Functions and Types
//
////////


std::string exec(const char* cmd);

void print_jets (const fastjet::ClusterSequence &, 
                 const vector<fastjet::PseudoJet> &);




////////
//
//  CoreJetAlgorithm
//
////////

class CoreJetAlgorithm : public JetDefinition::Plugin {
public:
  CoreJetAlgorithm (){};

  virtual bool supports_ghosted_passive_areas() const {return true;}
  virtual void set_ghost_separation_scale(double scale) const {};

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;                      
  virtual double R() const {return 0.0;}

private:

  // for processing the python output
  mutable std::vector<int> input_labels;
  mutable std::vector<int> remaining_labels;
  mutable std::vector<std::array<int, 3>> cluster_labels;
  mutable std::map<int, int> label_convertor;


  std::string GetKinematicsString(const ClusterSequence &) const;
  void add_inputs_to_converter(const ClusterSequence &) const;
  void process_output(const string &) const;

protected:
  void PrintJet(const PseudoJet& jet) const;
};

/// experiment
class SpectralLong : public CoreJetAlgorithm{
    public:
        SpectralLong(double unneeded);

  
        virtual std::string description () const;
        virtual double R() const {return 0.0;}
    private:
        double my_unneeded;

};


} // namespace contrib

FASTJET_END_NAMESPACE

#endif  // __FASTJET_CONTRIB_VARIABLER_HH__
