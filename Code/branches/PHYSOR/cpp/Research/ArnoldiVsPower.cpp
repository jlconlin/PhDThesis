/*ArnoldiVsPower.cpp
 $Author: jlconlin $
 $Id: ArnoldiVsPower.cpp 396 2008-10-11 00:59:20Z jlconlin $
 $Revision: 396 $
 $Date: 2008-10-10 18:59:20 -0600 (Fri, 10 Oct 2008) $

 This file is for comparing Arnoldi's method to the power method.

 */


#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
#include "boost/program_options.hpp"
#include "ArnoldiMC.h"
#include "Power.h"
#include "Material.h"
#include "gnuplot_i.hpp"
#include "Field.hh"
#include "OneDCartMesh.hh"
#include "HistSource.h"
#include "gnuFile.h"
namespace po = boost::program_options;

using std::cout;
using std::endl;
using std::string;

int ParseCommandLine(int, char**, po::variables_map&);

int main( int argc, char* argv[]){
    int PCL = 0;    // ParseCommandLine results
    po::variables_map options;

    PCL = ParseCommandLine(argc, argv, options);
    if( PCL ) return PCL;   // Return error from parser

    // Make command line arguments into string
    std::ostringstream cmd;
    for( int i = 0; i < argc; ++i ){
        cmd << " " << argv[i];
    }

    string filename = options["file"].as<string>();
//  string filename;
    int inactive = options["inactive"].as<int>();
    string inMethod = options["inMethod"].as<string>(); toupper(inMethod);
    int active = options["active"].as<int>();
    int iterations = options["iterations"].as<int>();
    int histories = options["histories"].as<int>();
    int zones = options["zones"].as<int>();
    int bins = options["bins"].as<int>();
    int numvectors = options["vectors"].as<int>();
    double width = options["width"].as<double>();
    int cxLabs = options["cxLabs"].as<int>();
    int cxMeanVals = options["cxMeanVals"].as<int>();
    int cxEstVals = options["cxEstVals"].as<int>();
    double relaxed = options["relaxed"].as<double>();
    bool RunPower = options["power"].as<bool>();
    bool run = options["run"].as<bool>();
    bool verbose = options["verbose"].as<bool>();
    cout << "options:\n"
         << "\tfilename:\t" << filename << "\n"
         << "\tinactive:\t" << inactive << "\n"
         << "\tinMethod:\t" << inMethod << "\n"
         << "\tactive:\t\t" << active << "\n"
         << "\titerations:\t" << iterations << "\n"
         << "\thistories:\t" << histories << "\n"
         << "\tzones:\t\t" << zones << "\n"
         << "\tbins:\t\t" << bins << "\n"
         << "\tvectors:\t" << numvectors <<"\n"
         << "\tcxMeanVals:\t" << cxMeanVals << "\n"
         << "\tcxLabs:\t\t" << cxLabs << "\n"
         << "\tcxEstVals:\t" << cxEstVals << "\n"
         << "\trelaxed:\t" << relaxed << "\n"
         << "\twidth:\t\t" << width << "\n"
         << "\tpower:\t\t" << RunPower << "\n"
         << "\trun:\t\t" << run << "\n"
         << "\tverbose:\t" << verbose << "\n";
    

    vector<unsigned long> seed(2);
    if( options["seed"].as<unsigned long>() != 0 ){
        seed[0] = options["seed"].as<unsigned long>();
    }
    else {
        seed[0] = RandomLib::Random::SeedWord();
        // Add seed to cmd
        cmd << " seed = " << seed[0];
    }
    seed[1] = 1;
    std::cout << "Master seed: " << seed[0] << "\n";

    // Geometry
    Mesh geoMesh(width, 1.0, zones);
    vector<Material> Mat(zones, Material(0.5,0.5,0.0,1.0) );
    Field<Zone, Material> Geometry(geoMesh, Mat);

    // Initial Source
    Mesh DiscMesh(width, 1.0, bins);    // Discretization Mesh
    vector<double> edges = DiscMesh.ZoneCenters();
    vector<double> SourceProbability(bins, 1.0);
    Field<Zone, double> SourceField(DiscMesh, SourceProbability);

    if( run ){
        if( filename == "" ){
            cout << "\n---------------------------------------------------"
                 << "\nYou can't run this without specifying a filename.\n"
                 << "Otherwise you are wasting your time." << endl;
            return 1;
        }
        gnuFile Output(filename);
        Output.appendH(cmd.str());

        // Plot results
        Gnuplot GValues("linespoints", false);
        GValues.set_title(cmd.str());
        GValues.set_xlabel("Active Histories");
        GValues.set_ylabel("Mean Eigenvalue Estimates");
        GValues.cmd("set yrange [0.95:1.02]");
        Gnuplot GVectors("histeps", false);
        GVectors.set_title(cmd.str());
        GVectors.set_xlabel("Slab Width (mfp)");
        GVectors.set_ylabel("Mean Eigenvector Estimate");

        Power<BankSource> PMC(seed, Geometry, DiscMesh);
        ArnoldiMC AMC(seed, Geometry, DiscMesh, relaxed);

        // Arnoldi's method
        HistSource HSource(seed, SourceField);
        cout << "\n\n-------------------\nRunning Arnoldi's method...\n";
        AMC.ERAM(HSource, inactive, active, iterations, histories);

        // Get AMC data
        vector<double> AMCx = AMC.HperR();
        vector<double> AMCMeanValue; vector<double> AMCStdDevValue;
        AMCMeanValue = AMC.MeanValues(0);
        AMCStdDevValue = AMC.StdDevValues(0);
        GValues.plot_xy_err(AMCx, AMCMeanValue, AMCStdDevValue, "Arnoldi");
        Output.append(string("Arnoldi eValues-0"), AMCx, AMCMeanValue,
                    AMCStdDevValue);
        AMCMeanValue = AMC.MeanValues(1);
        AMCStdDevValue = AMC.StdDevValues(1);
        Output.append(string("Arnoldi eValues-1"), AMCx, AMCMeanValue,
                    AMCStdDevValue);

        if( width == 20.0 ) GValues.cmd("replot 0.985928 lc -1 notitle");
        else if( width == 50.0 ) GValues.cmd("replot 0.997520 lc -1 notitle");
        else if( width == 100.0 ) GValues.cmd("replot 0.999333 lc -1 notitle");

        if( numvectors ){   // Plot eigenvectors
            vector<double> MeanRitz; vector<double> StdDevRitz;
            cout << "\nArnoldi eigenvectors:";
            for( int i = 0; i < numvectors; ++i ){
                AMC.RealMeanVectors(i, MeanRitz, StdDevRitz);
                Norm(MeanRitz);
                GVectors.plot_xy(edges, MeanRitz, "Arnoldi");
                ostringstream VecTitle;  VecTitle << "Arnoldi vector-" << i;
                Output.append(VecTitle.str(), edges, MeanRitz, StdDevRitz);
            }
        }

        if( cxMeanVals > 0){
            // Print Complex Eigenvalues
            Gnuplot CxMeanVal("points", false);
            std::ostringstream MeanTitle; 
            MeanTitle << cmd.str() << " Mean Values";
            CxMeanVal.set_title(MeanTitle.str());
            CxMeanVal.set_xlabel("Real Part"); CxMeanVal.set_ylabel("Imag Part");
            CxMeanVal << "set xzeroaxis" 
                  << "set yzeroaxis" 
                  << "set key bottom left";
            vector<double> RealPart; vector<double> ImagPart;

            for( int i = 0; i < cxMeanVals; ++i ){
                AMC.RealMeanValues(i, RealPart, AMCStdDevValue);
                AMC.ImagMeanValues(i, ImagPart, AMCStdDevValue);
                std::ostringstream title; title << "lambda-" << i;
                CxMeanVal.plot_xy(RealPart, ImagPart, title.str());
                sleep(1);
            }
        }
        if( cxEstVals > 0){
            // Print Complex Eigenvalues
            Gnuplot CxEstVal("points", false);
            std::ostringstream EstTitle; 
            EstTitle << cmd.str() << " Estimated Values";
            CxEstVal.set_title(EstTitle.str());
            CxEstVal.set_xlabel("Real Part"); CxEstVal.set_ylabel("Imag Part");
            CxEstVal << "set xzeroaxis" 
                  << "set yzeroaxis" 
                  << "set key bottom left";
            vector<double> RealPart; vector<double> ImagPart;

            for( int i = 0; i < cxEstVals; ++i ){
                SplitComplexVector(AMC.CycleValues()[i], RealPart,
                        ImagPart);
                std::ostringstream title; title << "lambda-" << i;
                CxEstVal.plot_xy(RealPart, ImagPart, title.str());
                sleep(1);
            }
        }
        if( cxLabs > 0 ){
            // Print Complex Eigenvalues
            Gnuplot CxLabels("labels", false);
            
            std::ostringstream EstTitle; 
            EstTitle << cmd.str() << " Estimated Values";
            CxLabels.set_title(EstTitle.str());

            CxLabels.set_xlabel("Real Part"); CxLabels.set_ylabel("Imag Part");
            CxLabels << "set xzeroaxis" 
                  << "set yzeroaxis" 
                  << "set key bottom left";
            vector<double> RealPart; vector<double> ImagPart;

            // Plot complex values in order generated
            for( int i = 0; i < cxLabs; ++i ){
                SplitComplexVector(AMC.CycleValues()[i], RealPart,
                        ImagPart);

                vector<string> Labels(RealPart.size());
                vector<string>::iterator LabIter = Labels.begin();
                unsigned int j = 0;
                for( ; LabIter < Labels.end(); ++LabIter, ++j ){
                    ostringstream label; label << j;
                    *LabIter = label.str();
                }
                std::ostringstream title; title << "lambda-" << i;
                CxLabels.plot_xy_labels(RealPart, ImagPart, Labels, title.str());
                sleep(1);
            }
        }

        // Power method
        if( RunPower ){
            cout << "\n\nRunning power method...\n";
            BankSource* BSource = new BankSource(SourceField, DiscMesh, seed, histories);

            int icycles = inactive*iterations;  // Number of inactive cycles
            int acycles = active*iterations; // Number of active cycles

            // Average number of histories per cycles
            int pHistories = AMC.HperR().back()/(icycles + acycles);

            PMC.power(BSource, acycles, icycles, pHistories);
              
            // Get PMC data
            vector<double> PMCx = PMC.HperI();
            vector<double> MeanV = PMC.MeanValues();
            vector<double> SDV = PMC.StdDevValues();
            GValues.plot_xy_err(PMCx, MeanV, SDV, "Power");
            Output.append(string("Power eValues"), PMCx, MeanV, SDV);
            if( numvectors ){
                MeanV = PMC.MeanVector(); Norm(MeanV);
                SDV = PMC.StdDevVector();
                GVectors.plot_xy(edges, MeanV, "Power");
                Output.append(string("Power vector"), edges, MeanV, SDV);
            }
        }

        sleep(1);
    }

    return 0;
}

int ParseCommandLine(int argc, char* argv[], po::variables_map& vm){
    try {
        string Usage("usage: ArnoldiVsPower [options]\nAvailable options:");
        po::options_description desc(Usage);
        desc.add_options()
            ("help", "produce help message")
            ("file", po::value<string>()->default_value(""),
                "Filename of output suitable for Gnuplot.")
            ("inactive", po::value<int>()->default_value(2),
                "Number of inactive Arnoldi restarts")
            ("inMethod", po::value<string>()->default_value("arnoldi"),
                "Algorithmic method for inactive Arnoldi")
            ("active", po::value<int>()->default_value(20),
                "Number of active Arnoldi restarts")
            ("iterations", po::value<int>()->default_value(5),
                "Number of iterations per restart")
            ("histories", po::value<int>()->default_value(5000), 
                "Number of histories per iteration")
            ("zones", po::value<int>()->default_value(1),
                "Number of geometric zones")
            ("bins", po::value<int>()->default_value(50),
                "Number of discretization bins")
            ("width", po::value<double>()->default_value(10.0),
                "Width of slab.")
            ("seed", po::value<unsigned long>()->default_value(0.0),
                "Random Number Generator seed")
            ("vectors", po::value<int>()->default_value(2),
                "Number of eigenvectors to print")
            ("cxLabs", po::value<int>()->default_value(0),
                "Complex Arnoldi values estimates plotted as indices.")
            ("cxEstVals", po::value<int>()->default_value(0),
                "Flag if complex Arnoldi values estimates should be plotted.")
            ("cxMeanVals", po::value<int>()->default_value(0),
                "Flag if the mean complex Arnoldi values should be plotted.")
            ("relaxed", po::value<double>()->default_value(false),
                "Relaxation constant for relaxed Arnoldi.")
            ("power", po::value<bool>()->default_value(1),
                "Run power method.")
            ("run", po::value<bool>()->default_value(false),
                "Flag to determine if program should be run.")
            ("verbose", po::value<bool>()->default_value(true),
                "Flag to determine if output should be verbose")
        ;

        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
    }

    return 0;
}

