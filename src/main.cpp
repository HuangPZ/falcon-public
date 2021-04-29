#include <iostream>
#include <string>
#include "AESObject.h"
#include "Precompute.h"
#include "secondary.h"
#include "connect.h"
#include "NeuralNetConfig.h"
#include "NeuralNetwork.h"
#include "unitTests.h"
#include <fstream>


int partyNum;
AESObject* aes_indep;
AESObject* aes_next;
AESObject* aes_prev;
Precompute PrecomputeObject;

extern int MatMul_Com,ReLU_Com,MP_Com,MP_ReLU_Com,Div_Com,BN_Com;
extern CommunicationObject commObject;
int main(int argc, char** argv)
{
	string Network_l[6]={"SecureML", "Sarda", "MiniONN", "LeNet", "AlexNet", "VGG16"};
	string Dataset_l[3]={"MNIST", "CIFAR10", "ImageNet"};
	string network, dataset, security;
	security = "Semi-honest";
	for (int networki = 0; networki < 6; networki++) {
		for (int dataseti = 0; dataseti < 3; dataseti++){

			network = Network_l[networki];
			dataset = Dataset_l[dataseti];
			// cout << network << dataset << (network=="SecureML"||network=="Sarda"||network=="MiniONN"||network=="LeNet")
			// <<(dataset!="MNIST")<<((network=="SecureML"||network=="Sarda"||network=="MiniONN"||network=="LeNet")&&(dataset!="MNIST"))
			//  << endl;
			if((network=="SecureML"||network=="Sarda"||network=="MiniONN"||network=="LeNet")&&(dataset!="MNIST")){
				continue;
			} 
			else if ((network=="AlexNet"||network=="VGG16")&&(dataset=="MNIST")){
				continue;
			}
			else if (dataset=="MNIST"||(dataset=="CIFAR10")){
				continue; //TODO: to be deleted
			}
			cout << network << dataset << security<< endl;

		/****************************** PREPROCESSING ******************************/ 
			parseInputs(argc, argv);
			NeuralNetConfig* config = new NeuralNetConfig(NUM_ITERATIONS);
			
			bool PRELOADING = false;

		/****************************** SELECT NETWORK ******************************/ 
			//Network {SecureML, Sarda, MiniONN, LeNet, AlexNet, and VGG16}
			//Dataset {MNIST, CIFAR10, and ImageNet}
			//Security {Semi-honest or Malicious}
			// if (argc == 9)
			// {network = argv[6]; dataset = argv[7]; security = argv[8];}
			// else
			// {
			// 	network = "AlexNet";//"VGG16";
			// 	dataset = "CIFAR10";//"CIFAR10";
			// 	security = "Semi-honest";
			// }
			selectNetwork(network, dataset, security, config);
			config->checkNetwork();

			NeuralNetwork* net = new NeuralNetwork(config);

		/****************************** AES SETUP and SYNC ******************************/ 
			aes_indep = new AESObject(argv[3]);
			aes_next = new AESObject(argv[4]);
			aes_prev = new AESObject(argv[5]);
			cout<<1<<endl;
			initializeCommunication(argv[2], partyNum);
			cout<<1<<endl;
			synchronize(2000000);
			cout<<1<<endl;

		/****************************** RUN NETWORK/UNIT TESTS ******************************/ 
			//Run these if you want a preloaded network to be tested
			//assert(NUM_ITERATION == 1 and "check if readMiniBatch is false in test(net)")
			//First argument {SecureML, Sarda, MiniONN, or LeNet}
			// network += " preloaded"; PRELOADING = true;
			// preload_network(PRELOADING, network, net);

			start_m();
			//Run unit tests in two modes: 
			//	1. Debug {Mat-Mul, DotProd, PC, Wrap, ReLUPrime, ReLU, Division, BN, SSBits, SS, and Maxpool}
			//	2. Test {Mat-Mul1, Mat-Mul2, Mat-Mul3 (and similarly) Conv*, ReLU*, ReLUPrime*, and Maxpool*} where * = {1,2,3}
			// runTest("Debug", "BN", network);
			// runTest("Test", "ReLUPrime1", network);

			// Run forward/backward for single layers
			//  1. what {F, D, U}
			// 	2. l {0,1,....NUM_LAYERS-1}
			// size_t l = 0;
			// string what = "F";
			// runOnly(net, l, what, network);

			//Run training
			network += " train";
			train(net);

			//Run inference (possibly with preloading a network)
			// network += " test";
			// test(PRELOADING, network, net);
			ofstream myfile;
			string filename;
			filename = "Falcon_comm_"+security+"_"+network+"_"+dataset+"_"+".txt";
			myfile.open (filename.c_str());
			myfile << "----------------------------------------------" << endl;  	
			myfile << "Run details: " << NUM_OF_PARTIES << "PC (P" << partyNum 
				<< "), " << NUM_ITERATIONS << " iterations, batch size " << MINI_BATCH_SIZE << endl 
				<< "Running " << security << " " << network << " on " << dataset << " dataset" << endl;
			myfile << "----------------------------------------------" << endl << endl; 
			myfile.close();	
			string filename1 = "Falcon_comm_"+security+"_"+network+"_"+dataset+"_"+"1.txt";
			myfile.open (filename.c_str());
			myfile << endl;
			myfile.close();	
			end_m(network,filename1);
			// cout << "----------------------------------------------" << endl;  	
			// cout << "Run details: " << NUM_OF_PARTIES << "PC (P" << partyNum 
			// 	 << "), " << NUM_ITERATIONS << " iterations, batch size " << MINI_BATCH_SIZE << endl 
			// 	 << "Running " << security << " " << network << " on " << dataset << " dataset" << endl;
			// cout << "----------------------------------------------" << endl << endl;  

			// cout << "MatMul_Com: "<< MatMul_Com << endl;
			// cout << "ReLU_Com: "<< ReLU_Com << endl;
			// cout << "MP_Com: "<< MP_Com << endl;
			// cout << "MP_ReLU_Com: "<< MP_ReLU_Com << endl;
			// cout << "BN_Com: "<< BN_Com << endl;
			// cout << "Div_Com: "<< Div_Com << endl;
			myfile.open (filename.c_str(),fstream::app);
			myfile << "P" << partyNum << endl;
			myfile << "MatMul_Com: "<< MatMul_Com << endl;
			myfile << "ReLU_Com: "<< ReLU_Com << endl;
			myfile << "MP_Com: "<< MP_Com << endl;
			myfile << "MP_ReLU_Com: "<< MP_ReLU_Com << endl;
			myfile << "BN_Com: "<< BN_Com << endl;
			myfile << "Div_Com: "<< Div_Com << endl;



			myfile.close();



			printNetwork(net);

		/****************************** CLEAN-UP ******************************/ 
			delete aes_indep;
			delete aes_next;
			delete aes_prev;
			delete config;
			delete net;
			deleteObjects();
	}
	}

	return 0;
}




