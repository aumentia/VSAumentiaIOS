/*************************************************************************
 *
 * AUMENTIA TECHNOLOGIES
 * __________________
 *
 *  Copyright Aumentia Technologies. All rights reserved 2015.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Aumentia Technologies and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Aumentia Technologies
 * and its suppliers and may be covered by Spain and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Aumentia Technologies.
 *
 * File: Roi.h
 * Description:
 * Author: Pablo GM (info@aumentia.com)
 * Created: 14/01/15.
 * Version 1.11
 *
 *
 **************************************************************************/

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

__attribute__((__visibility__("default"))) @interface Roi : NSObject

/**
 * Region of interest bounds
 */
@property (assign, nonatomic) CGRect roiRect;

/**
 * QR/bar code scanned string.
 */
@property (strong, nonatomic) NSString* qrString;

/**
 * Init a ROI instance
 * @param rect ROI bounds
 */
-(id)initWithRect:(CGRect)rect;

@end
