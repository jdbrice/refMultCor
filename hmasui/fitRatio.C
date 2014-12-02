

void fitRatio(){
	
	TFile * f = new TFile( "res.root", "READ" );

	TH1D* sim = (TH1D*)f->Get( "hRefMultSim" )->Clone( "sim" );
	TH1D* ratio = (TH1D*)f->Get( "hRefMultSim" )->Clone( "sim" );
	TH1D* data = (TH1D*)f->Get( "hRefMultTpc" )->Clone( "data" );
	ratio->Divide( data );

	TF1 * f1 = new TF1( "f1", "[0] + [1]/( [2]*x + [3] ) + [4]*([2]*x+[3])", 0, 50);
	f1->SetParameters( 1, 100, 50 );

	ratio->Fit( "f1", "QR" );

	TH1D* nData = (TH1D*)data->Clone("nData");

	for ( int i = 1; i < data->GetXaxis()->GetNbins(); i++ ){
		if ( i > 60 )
			continue;
		double v = nData->GetBinContent( i );
		nData->SetBinContent( i, v * f1->Eval( i ) );
	}

	nData->Draw();
	sim->Draw("SAME");




}