//
//  ViewController.h
//  HelloVisualSearch
//
//  Copyright (c) 2015 Aumentia. All rights reserved.
//
//  Written by Pablo GM <info@aumentia.com>, January 2015
//

#import <UIKit/UIKit.h>
#import <VS/VS.h>
#import "CaptureSessionManager.h"

@interface ViewController : UIViewController<imageMatchedProtocol, QRMatchedProtocol, CameraCaptureDelegate>

@end
