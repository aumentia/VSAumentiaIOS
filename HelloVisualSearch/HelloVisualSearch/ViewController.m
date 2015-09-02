//
//  ViewController.m
//  HelloVisualSearch
//
//  Copyright (c) 2015 Aumentia. All rights reserved.
//
//  Written by Pablo GM <info@aumentia.com>, January 2015
//

#import "ViewController.h"


///////////////
#define VSLog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);

#define VSAlert(fmt, ...)  { UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Result" message:[NSString stringWithFormat:fmt, ##__VA_ARGS__]  delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil]; [alert show]; }
///////////////


@interface ViewController ()
{
    vsPlugin                    *_myVs;
    UIAlertView                 *_myLoading;
    CaptureSessionManager       *_captureManager;
    UIView                      *_cameraView;
    UIImageView                 *_resPic;
}

@end

@implementation ViewController


#pragma mark - View Life Cycle

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidLoad];
	
    UIImage *myLogo         = [UIImage imageNamed:@"aumentia®.png"];
    UIImageView *myLogoView = [[UIImageView alloc] initWithImage:myLogo];
    [myLogoView setFrame:CGRectMake(0, 0, 150, 61)];
    [self.view addSubview:myLogoView];
    [self.view bringSubviewToFront:myLogoView];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self initCapture];
    
    [self addVisualSearch];
    
    [self addLoading];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
       
        [self addImages];
        
    });
    
    ///// TESTS //////
    //[self addCropRect];
    
    //[self addQRRois];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self removeCapture];
    
    [self removeImages];
    
    [self removeVisualSearch];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Add / Remove Images

- (void)addImages
{
    @autoreleasepool
    {
        /// LOCAL
        for (int i = 1; i <= 6; i++)
        {
            NSString *imageName = [NSString stringWithFormat:@"pic%d.jpg", i];
            BOOL res            = [_myVs insertImage:[UIImage imageNamed:imageName] withId:i];
            
            VSLog(@"Image %@ --> %@", imageName, res == YES ? @"ADDED" : @"NOT ADDED");
        }
        
        /// REMOTE
        NSInteger resId = [_myVs insertImageFromURL:[NSURL URLWithString:@"https://s3-us-west-1.amazonaws.com/aumentia/pic_from_url.jpg"]];
        
        if(resId == -1) VSLog(@"Error adding image from URL")
        else            VSLog(@"Image from URL added with id %ld", (long)resId);
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [self removeLoading];
        });
    }
}

- (void)removeImages
{
    [_myVs deleteAllImages];
}


#pragma mark - Loading

- (void)addLoading
{
    //Add images into the library
    _myLoading = [[UIAlertView alloc] initWithTitle:@"Loading..." message:nil delegate:nil cancelButtonTitle:nil otherButtonTitles:nil];
    UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    spinner.center = CGPointMake(142,70);
    [spinner startAnimating];
    [_myLoading addSubview:spinner];
    [_myLoading show];
}

- (void)removeLoading
{
    [_myLoading dismissWithClickedButtonIndex:0 animated:YES];
    _myLoading  = nil;
}


#pragma mark - VS Life Cycle

- (void)addVisualSearch
{
    if ( !_myVs )
    {
        // Init
        _myVs = [[vsPlugin alloc] initWithKey:@"c2b255434d1d71ac23f62fb316b54e95d1e01cd8" setDebug:YES];
        
        // Set delegates
        [_myVs setImageDelegate:self];
        [_myVs setQrDelegate:self];
        
        // Set detection threshold
        [_myVs setMatchingThreshold:8];
        
        // Set mode: search for QR and / or bar codes and images
        [_myVs setSearchMode:search_all];
        
        // Add motion filter
        [_myVs initMotionDetectionWithThreshold:3 enableDebugLog:NO];
        
        // Some more settings
//        [_myVs setFilterWindow:5];
//        [_myVs setMaxFeatures:50];
//        [_myVs setFrameSize:250];
        
    }
}

- (void)removeVisualSearch
{
    if(_myVs != nil)
    {
        [_myVs removeMotionDetection];
        
        _myVs.imageDelegate = nil;
        _myVs.qrDelegate    = nil;
        _myVs               = nil;
    }
}


#pragma mark - ROIs and Crop Rects

- (void)addRectToView:(CGRect)rect
{
    // Add frame capturer
    UIView* frameView = [[UIView alloc] initWithFrame:rect];
    frameView.backgroundColor = [UIColor clearColor];
    frameView.layer.borderColor = [self colorWithHexString:@"009ee0"].CGColor;
    frameView.layer.borderWidth = 3.0f;
    [self.view addSubview:frameView];
}

- (void)addQRRois
{
    // Add regions to match several QR / bar codes
    Roi *ROI1 = [[Roi alloc] initWithRect:CGRectMake(0, 0, 160, 240)];
    Roi *ROI2 = [[Roi alloc] initWithRect:CGRectMake(0, 240, 160, 240)];
    Roi *ROI3 = [[Roi alloc] initWithRect:CGRectMake(160, 0, 160, 240)];
    Roi *ROI4 = [[Roi alloc] initWithRect:CGRectMake(160, 240, 160, 240)];
    
    // Draw the regions
    [self addRectToView:CGRectMake(0, 0, 160, 240)];
    [self addRectToView:CGRectMake(0, 240, 160, 240)];
    [self addRectToView:CGRectMake(160, 0, 160, 240)];
    [self addRectToView:CGRectMake(160, 240, 160, 240)];
    
    // Add them to the system
    [_myVs addQRRect:ROI1];
    [_myVs addQRRect:ROI2];
    [_myVs addQRRect:ROI3];
    [_myVs addQRRect:ROI4];
}

- (void)addCropRect
{
    CGRect myRect = CGRectMake(20, 20, 200, 125);
    
    [self addRectToView:myRect];
    
    [_myVs setImageCropRect:myRect];
}


#pragma mark - External Camera Delegates

- (void)processNewCameraFrameRGB:(CVImageBufferRef)cameraFrame
{
    [_myVs processRGBFrame:cameraFrame saveImageToPhotoAlbum:NO ];
}

- (void)processNewCameraFrameYUV:(CVImageBufferRef)cameraFrame
{
    [_myVs processYUVFrame:cameraFrame saveImageToPhotoAlbum:NO];
}


#pragma mark - Visual Search Delegates

- (void)imageMatchedResult:(NSInteger)uId
{
    if (uId != -1)
    {
        VSLog(@"Image detected --> %ld", (long)uId);
        
        dispatch_async(dispatch_get_main_queue(), ^{
            
            [_resPic setHidden:NO];
            
            if(_resPic == nil)
            {
                _resPic = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[NSString stringWithFormat:@"pic%ld.jpeg", (long)uId]]];
                [_resPic setFrame:CGRectMake(0, self.view.frame.size.height - 70, 100, 63)];
                [self.view addSubview:_resPic];
            }
            else
            {
                [_resPic setImage:[UIImage imageNamed:[NSString stringWithFormat:@"pic%ld.jpeg", (long)uId]]];
            }
            
            //VSAlert(@"Image detected --> %d", uId);
        });
    }
    else
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (_resPic != nil)
            {
                [_resPic setHidden:YES];
            }
        });
    }
}

- (void)singleQRMatchedResult:(NSString *)res
{
    if ( ![res isEqualToString:@""])
    {
        VSLog(@"QR / bar code detected --> %@", res);
        
        dispatch_async(dispatch_get_main_queue(), ^{
            VSAlert(@"QR / bar code detected --> %@", res);
        });
    }
}

- (void)multipleQRMatchedResult:(NSArray *)codes
{
    NSString *code = @"";
    
    for (int i = 0; i < [codes count]; i++)
    {
        Roi *roi = [codes objectAtIndex:i];
        
        code = [code stringByAppendingString:roi.qrString];
    }
    
    VSLog(@"Multiple QR / bar codes detected --> %@", code);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        VSAlert(@"Multiple QR / bar codes detected --> %@", code);
    });
}


#pragma mark - Autorotate

- (BOOL)shouldAutorotate
{
    return NO;
}


#pragma mark - Add external camera

- (void)initCapture {
	
    // init capture manager
    _captureManager = [[CaptureSessionManager alloc] init];
    
    _captureManager.delegate = self;
    
    // set video streaming quality
    _captureManager.captureSession.sessionPreset = AVCaptureSessionPresetPhoto;
    
    [_captureManager setOutPutSetting:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA]]; //kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange, kCVPixelFormatType_420YpCbCr8BiPlanarFullRange or kCVPixelFormatType_32BGRA
    
    [_captureManager addVideoInput:AVCaptureDevicePositionBack]; //AVCaptureDevicePositionFront / AVCaptureDevicePositionBack
    [_captureManager addVideoOutput];
    [_captureManager addVideoPreviewLayer];
    
    CGRect layerRect = self.view.bounds;
    
    [[_captureManager previewLayer] setOpaque: 0];
    [[_captureManager previewLayer] setBounds:layerRect ];
    [[_captureManager previewLayer] setPosition:CGPointMake( CGRectGetMidX(layerRect), CGRectGetMidY(layerRect) ) ];
    
    // create a view, on which we attach the AV Preview layer
    _cameraView = [[UIView alloc] initWithFrame:self.view.bounds];
    [[_cameraView layer] addSublayer:[_captureManager previewLayer]];
    
    // add the view we just created as a subview to the View Controller's view
    [self.view addSubview: _cameraView];
    
    // start !
    dispatch_async(dispatch_get_main_queue(), ^{
        [self start_captureManager];
    });
}

- (void)removeCapture
{
    [_captureManager.captureSession stopRunning];
    [_cameraView removeFromSuperview];
    _captureManager     = nil;
    _cameraView         = nil;
}

- (void)start_captureManager
{
    @autoreleasepool
    {
        [[_captureManager captureSession] startRunning];
    }
}

#pragma mark - Util functions

-(UIColor*)colorWithHexString:(NSString*)hex
{
    NSString *cString = [[hex stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
    
    // String should be 6 or 8 characters
    if ([cString length] < 6) return [UIColor grayColor];
    
    // strip 0X if it appears
    if ([cString hasPrefix:@"0X"]) cString = [cString substringFromIndex:2];
    
    if ([cString length] != 6) return  [UIColor grayColor];
    
    // Separate into r, g, b substrings
    NSRange range;
    range.location = 0;
    range.length = 2;
    NSString *rString = [cString substringWithRange:range];
    
    range.location = 2;
    NSString *gString = [cString substringWithRange:range];
    
    range.location = 4;
    NSString *bString = [cString substringWithRange:range];
    
    // Scan values
    unsigned int r, g, b;
    [[NSScanner scannerWithString:rString] scanHexInt:&r];
    [[NSScanner scannerWithString:gString] scanHexInt:&g];
    [[NSScanner scannerWithString:bString] scanHexInt:&b];
    
    return [UIColor colorWithRed:((float) r / 255.0f)
                           green:((float) g / 255.0f)
                            blue:((float) b / 255.0f)
                           alpha:1.0f];
}

@end