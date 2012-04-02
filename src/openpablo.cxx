#include <cv.h>
#include <highgui.h>
 #include <QFile>


#include <Magick++.h>
#include <string>
#include <iostream>
#include <list>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
#include <exception>

#include <magick/MagickCore.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/string_path.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>


#include "json_spirit.h"
#include "lcms2.h"
#include "lcms2_plugin.h"


#include <podofo/podofo.h>



using namespace std;

using namespace json_spirit;
using namespace PoDoFo;

using namespace Magick;




const mValue& find_value( const mObject& obj, const string& name  )
{
    mObject::const_iterator i = obj.find( name );

    assert( i != obj.end() );
    assert( i->first == name );

    return i->second;
}



int main ( int argc, char **argv )
{
    using boost::property_tree::ptree;
    ptree pt;

    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    read_json("tests/onefile.txt" , pt);

    std::string inputFile = pt.get<std::string>("Input.InputFile");
    std::cout << inputFile << "\n";
    
return -1;
    
    
    
  
//    PdfStreamedDocument document( "tests/tmp.pdf" );

  QFile file("data/NKsRGB.icm");

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = file.readAll();
        file.close();
    }
    else
      std::cout << ("rfile failed...");
    
   //Initialize ImageMagick install location for Windows
  InitializeMagick(*argv);

  

  
    struct stat  st;
    int  fd;

    if ( (fd = open("data/NKsRGB.icm", O_RDONLY)) <0) {
	cerr << "unable to open original ICC file - " << strerror(errno) << endl;
	return 1;
    }
    fstat(fd, &st);
    const size_t  sRGBsz = st.st_size;
    char*  sRGB = new char[sRGBsz];
    read(fd, sRGB, sRGBsz);
    close(fd);


    if ( (fd = open("data/ISOcoated_v2_300_bas.ICC", O_RDONLY)) <0) {
	cerr << "unable to open target ICC file - " << strerror(errno) << endl;
	return 1;
    }
    fstat(fd, &st);
    const size_t  aRGBsz = st.st_size;
    char*  aRGB = new char[aRGBsz];
    read(fd, aRGB, aRGBsz);
    close(fd);


    try
    {
	Magick::Image  magick("data/cmyk_cmyk.jpg");

	magick.renderingIntent(Magick::PerceptualIntent);

	magick.profile("ICC", Magick::Blob(aRGB, aRGBsz));

	const Magick::Blob  targetICC(aRGB, aRGBsz);
	magick.profile("ICC", targetICC);
	magick.iccColorProfile(targetICC);

	magick.write("tests/srgb.jpg");
    }
      catch (const std::exception& ex)
    {
	cerr << "failed to magick - " << ex.what() << endl;
    }

    delete []  sRGB;
    delete []  aRGB;

return -1;  
  
  

  cvNamedWindow( "My Window", 1 );
  IplImage *img = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 1 );
  CvFont font;
  double hScale = 1.0;
  double vScale = 1.0;
  int lineWidth = 1;
  cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC,
              hScale, vScale, 0, lineWidth );
  cvPutText( img, "Hello World!", cvPoint( 200, 400 ), &font,
             cvScalar( 255, 255, 0 ) );
  cvShowImage( "My Window", img );
  //cvWaitKey();

  
  
  try {
    
    string srcdir("");
    if(getenv("SRCDIR") != 0)
      srcdir = getenv("SRCDIR");
    
    // Common font to use.
    string font = "Helvetica";

    list<Image> montage;

    {
      //
      // Read model & smile image.
      //
      cout << "Read images ..." << endl;

      Image model( srcdir + "model.miff" );
      model.label( "Magick++" );
      model.borderColor( "black" );
      model.backgroundColor( "black" );
    
      Image smile( srcdir + "smile.miff" );
      smile.label( "Smile" );
      smile.borderColor( "black" );
    
      //
      // Create image stack.
      //
      cout << "Creating thumbnails..." << endl;
    
      // Construct initial list containing seven copies of a null image
      Image null;
      null.size( Geometry(70,70) );
      null.read( "NULL:black" );
      list<Image> images( 7, null );
    
      Image example = model;
    
      // Each of the following follow the pattern
      //  1. obtain reference to (own copy of) image
      //  2. apply label to image
      //  3. apply operation to image
      //  4. append image to container

      cout << "  add noise ..." << endl;
      example.label( "Add Noise" );
      example.addNoise( LaplacianNoise );
      images.push_back( example );

      cout << "  add noise (blue) ..." << endl;
      example.label( "Add Noise\n(Blue Channel)" );
      example.addNoiseChannel( BlueChannel, PoissonNoise );
      images.push_back( example );

      cout << "  annotate ..." << endl;
      example = model;
      example.label( "Annotate" );
      example.density( "72x72" );
      example.fontPointsize( 18 );
      example.font( font );
      example.strokeColor( Color() );
      example.fillColor( "gold" );
      example.annotate( "Magick++", "+0+20", NorthGravity );
      images.push_back( example );
/*
      cout << "  blur ..." << endl;
      example = model;
      example.label( "Blur" );
      example.blur( 0, 1.5 );
      images.push_back( example );

      cout << "  blur red channel ..." << endl;
      example = model;
      example.label( "Blur Channel\n(Red Channel)" );
      example.blurChannel( RedChannel, 0, 3.0 );
      images.push_back( example );

      cout << "  border ..." << endl;
      example = model;
      example.label( "Border" );
      example.borderColor( "gold" );
      example.border( Geometry(6,6) );
      images.push_back( example );

      cout << "  channel ..." << endl;
      example = model;
      example.label( "Channel\n(Red Channel)" );
      example.channel( RedChannel );
      images.push_back( example );

      cout << "  charcoal ..." << endl;
      example = model;
      example.label( "Charcoal" );
      example.charcoal( );
      images.push_back( example );

      cout << "  composite ..." << endl;
      example = model;
      example.label( "Composite" );
      example.composite( smile, "+35+65", OverCompositeOp);
      images.push_back( example );

      cout << "  contrast ..." << endl;
      example = model;
      example.label( "Contrast" );
      example.contrast( false );
      images.push_back( example );

      cout << "  convolve ..." << endl;
      example = model;
      example.label( "Convolve" );
      {
        // 3x3 matrix
        const double kernel[] = { 1, 1, 1, 1, 4, 1, 1, 1, 1 };
        example.convolve( 3, kernel );
      }
      images.push_back( example );

      cout << "  crop ..." << endl;
      example = model;
      example.label( "Crop" );
      example.crop( "80x80+25+50" );
      images.push_back( example );

      cout << "  despeckle ..." << endl;
      example = model;
      example.label( "Despeckle" );
      example.despeckle( );
      images.push_back( example );

      cout << "  draw ..." << endl;
      example = model;
      example.label( "Draw" );
      example.fillColor(Color());
      example.strokeColor( "gold" );
      example.strokeWidth( 2 );
      example.draw( DrawableCircle( 60,90, 60,120 ) );
      images.push_back( example );

      cout << "  edge ..." << endl;
      example = model;
      example.label( "Detect Edges" );
      example.edge( );
      images.push_back( example );

      cout << "  emboss ..." << endl;
      example = model;
      example.label( "Emboss" );
      example.emboss( );
      images.push_back( example );

      cout << "  equalize ..." << endl;
      example = model;
      example.label( "Equalize" );
      example.equalize( );
      images.push_back( example );
    
      cout << "  explode ..." << endl;
      example = model;
      example.label( "Explode" );
      example.backgroundColor( "#000000FF" );
      example.implode( -1 );
      images.push_back( example );

      cout << "  flip ..." << endl;
      example = model;
      example.label( "Flip" );
      example.flip( );
      images.push_back( example );

      cout << "  flop ..." << endl;
      example = model;
      example.label( "Flop" );
      example.flop();
      images.push_back( example );

      cout << "  frame ..." << endl;
      example = model;
      example.label( "Frame" );
      example.frame( );
      images.push_back( example );

      cout << "  gamma ..." << endl;
      example = model;
      example.label( "Gamma" );
      example.gamma( 1.6 );
      images.push_back( example );

      cout << "  gaussian blur ..." << endl;
      example = model;
      example.label( "Gaussian Blur" );
      example.gaussianBlur( 0.0, 1.5 );
      images.push_back( example );

      cout << "  gaussian blur channel ..." << endl;
      example = model;
      example.label( "Gaussian Blur\n(Green Channel)" );
      example.gaussianBlurChannel( GreenChannel, 0.0, 1.5 );
      images.push_back( example );
    
      cout << "  gradient ..." << endl;
      Image gradient;
      gradient.size( "130x194" );
      gradient.read( "gradient:#20a0ff-#ffff00" );
      gradient.label( "Gradient" );
      images.push_back( gradient );
    
      cout << "  grayscale ..." << endl;
      example = model;
      example.label( "Grayscale" );
      example.quantizeColorSpace( GRAYColorspace );
      example.quantize( );
      images.push_back( example );
    
      cout << "  implode ..." << endl;
      example = model;
      example.label( "Implode" );
      example.implode( 0.5 );
      images.push_back( example );

      cout << "  level ..." << endl;
      example = model;
      example.label( "Level" );
      example.level( 0.20*QuantumRange, 0.90*QuantumRange, 1.20 );
      images.push_back( example );

      cout << "  level red channel ..." << endl;
      example = model;
      example.label( "Level Channel\n(Red Channel)" );
      example.levelChannel( RedChannel, 0.20*QuantumRange, 0.90*QuantumRange, 1.20 );
      images.push_back( example );

      cout << "  median filter ..." << endl;
      example = model;
      example.label( "Median Filter" );
      example.medianFilter( );
      images.push_back( example );

      cout << "  modulate ..." << endl;
      example = model;
      example.label( "Modulate" );
      example.modulate( 110, 110, 110 );
      images.push_back( example );

      cout << "  monochrome ..." << endl;
      example = model;
      example.label( "Monochrome" );
      example.quantizeColorSpace( GRAYColorspace );
      example.quantizeColors( 2 );
      example.quantizeDither( false );
      example.quantize( );
      images.push_back( example );

      cout << "  motion blur ..." << endl;
      example = model;
      example.label( "Motion Blur" );
      example.motionBlur( 0.0, 7.0,45 );
      images.push_back( example );
    
      cout << "  negate ..." << endl;
      example = model;
      example.label( "Negate" );
      example.negate( );
      images.push_back( example );
    
      cout << "  normalize ..." << endl;
      example = model;
      example.label( "Normalize" );
      example.normalize( );
      images.push_back( example );
    
      cout << "  oil paint ..." << endl;
      example = model;
      example.label( "Oil Paint" );
      example.oilPaint( );
      images.push_back( example );

      cout << "  ordered dither 2x2 ..." << endl;
      example = model;
      example.label( "Ordered Dither\n(2x2)" );
      example.randomThreshold( Geometry(2,2) );
      images.push_back( example );

      cout << "  ordered dither 3x3..." << endl;
      example = model;
      example.label( "Ordered Dither\n(3x3)" );
      example.randomThreshold( Geometry(3,3) );
      images.push_back( example );

      cout << "  ordered dither 4x4..." << endl;
      example = model;
      example.label( "Ordered Dither\n(4x4)" );
      example.randomThreshold( Geometry(4,4) );
      images.push_back( example );
    
      cout << "  ordered dither red 4x4..." << endl;
      example = model;
      example.label( "Ordered Dither\n(Red 4x4)" );
      example.randomThresholdChannel( Geometry(4,4), RedChannel);
      images.push_back( example );

      cout << "  plasma ..." << endl;
      Image plasma;
      plasma.size( "130x194" );
      plasma.read( "plasma:fractal" );
      plasma.label( "Plasma" );
      images.push_back( plasma );
    
      cout << "  quantize ..." << endl;
      example = model;
      example.label( "Quantize" );
      example.quantize( );
      images.push_back( example );

      cout << "  quantum operator ..." << endl;
      example = model;
      example.label( "Quantum Operator\nRed * 0.4" );
      example.quantumOperator( RedChannel,MultiplyEvaluateOperator,0.40 );
      images.push_back( example );

      cout << "  raise ..." << endl;
      example = model;
      example.label( "Raise" );
      example.raise( );
      images.push_back( example );
    
      cout << "  reduce noise ..." << endl;
      example = model;
      example.label( "Reduce Noise" );
      example.reduceNoise( 1.0 );
      images.push_back( example );
    
      cout << "  resize ..." << endl;
      example = model;
      example.label( "Resize" );
      example.zoom( "50%" );
      images.push_back( example );
    
      cout << "  roll ..." << endl;
      example = model;
      example.label( "Roll" );
      example.roll( "+20+10" );
      images.push_back( example );
    
      cout << "  rotate ..." << endl;
      example = model;
      example.label( "Rotate" );
      example.rotate( 45 );
      example.transparent( "black" );
      images.push_back( example );

      cout << "  scale ..." << endl;
      example = model;
      example.label( "Scale" );
      example.scale( "60%" );
      images.push_back( example );
    
      cout << "  segment ..." << endl;
      example = model;
      example.label( "Segment" );
      example.segment( 0.5, 0.25 );
      images.push_back( example );
    
      cout << "  shade ..." << endl;
      example = model;
      example.label( "Shade" );
      example.shade( 30, 30, false );
      images.push_back( example );
    
      cout << "  sharpen ..." << endl;
      example = model;
      example.label("Sharpen");
      example.sharpen( 0.0, 1.0 );
      images.push_back( example );
    
      cout << "  shave ..." << endl;
      example = model;
      example.label("Shave");
      example.shave( Geometry( 10, 10) );
      images.push_back( example );
    
      cout << "  shear ..." << endl;
      example = model;
      example.label( "Shear" );
      example.shear( 45, 45 );
      example.transparent( "black" );
      images.push_back( example );
    
      cout << "  spread ..." << endl;
      example = model;
      example.label( "Spread" );
      example.spread( 3 );
      images.push_back( example );
    
      cout << "  solarize ..." << endl;
      example = model;
      example.label( "Solarize" );
      example.solarize( );
      images.push_back( example );
    
      cout << "  swirl ..." << endl;
      example = model;
      example.backgroundColor( "#000000FF" );
      example.label( "Swirl" );
      example.swirl( 90 );
      images.push_back( example );

      cout << "  threshold ..." << endl;
      example = model;
      example.label( "Threshold" );
      example.threshold( QuantumRange/2.0 );
      images.push_back( example );

      cout << "  threshold random ..." << endl;
      example = model;
      example.label( "Random\nThreshold" );
      example.randomThreshold( Geometry((size_t) (0.3*QuantumRange),
        (size_t) (0.85*QuantumRange)) );
      images.push_back( example );
    
      cout << "  unsharp mask ..." << endl;
      example = model;
      example.label( "Unsharp Mask" );
      //           radius_, sigma_, amount_, threshold_
      example.unsharpmask( 0.0, 1.0, 1.0, 0.05);
      images.push_back( example );
    
      cout << "  wave ..." << endl;
      example = model;
      example.label( "Wave" );
      example.matte( true );
      example.backgroundColor( "#000000FF" );
      example.wave( 25, 150 );
      images.push_back( example );
*/    
      //
      // Create image montage.
      //
      cout <<  "Montage images..." << endl;

      for_each( images.begin(), images.end(), fontImage( font ) );
      for_each( images.begin(), images.end(), strokeColorImage( Color("#600") ) );

      MontageFramed montageOpts;
      montageOpts.geometry( "130x194+10+5>" );
      montageOpts.gravity( CenterGravity );
      montageOpts.borderColor( "green" );
      montageOpts.borderWidth( 1 );
      montageOpts.tile( "7x4" );
      montageOpts.compose( OverCompositeOp );
      montageOpts.backgroundColor( "#ffffff" );
      montageOpts.font( font );
      montageOpts.pointSize( 18 );
      montageOpts.fillColor( "#600" );
      montageOpts.strokeColor( Color() );
      montageOpts.compose(OverCompositeOp);
      montageOpts.fileName( "Magick++ Demo" );
      montageImages( &montage, images.begin(), images.end(), montageOpts );
    }

    Image& montage_image = montage.front();
    {
      // Create logo image
      cout << "Adding logo image ..." << endl;
      Image logo( "logo:" );
      logo.zoom( "45%" );

      // Composite logo into montage image
      Geometry placement(0,0,(montage_image.columns()/2)-(logo.columns()/2),0);
      montage_image.composite( logo, placement, OverCompositeOp );
    }

    for_each( montage.begin(), montage.end(), depthImage(8) );
    for_each( montage.begin(), montage.end(), matteImage( false ) );
    for_each( montage.begin(), montage.end(), compressTypeImage( RLECompression) );

    cout << "Writing image \"demo_out.miff\" ..." << endl;
    writeImages(montage.begin(),montage.end(),"demo_out_%d.miff");

    // Uncomment following lines to display image to screen
    //    cout <<  "Display image..." << endl;
    //    montage_image.display();

  }
  catch( std::exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
      return 1;
    }

  return 0;
}
