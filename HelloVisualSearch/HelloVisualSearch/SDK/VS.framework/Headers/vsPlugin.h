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
 * File: vsPlugin.h
 * Description:
 * Author: Pablo GM (info@aumentia.com)
 * Created: 14/01/15.
 * Verion 1.03
 *
 *
 **************************************************************************/

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "_vs_versions.h"

@class Roi;

#ifndef _vsPlugin
#define _vsPlugin

typedef enum
{
    search_image,
    search_QRcodes,
    search_all
}search_mode;


/**
 *  Optional protocol Called when an Image is recognised within a pool.
 */
@protocol imageMatchedProtocol <NSObject>

@optional

/**
 *  Called when an Image is recognised within a pool.
 *  @param uId Matched Image unique Id.
 */
-(void)imageMatchedResult:(NSInteger)uId ;

-(void)trainedImageAdded:(NSInteger)uId;

@end


/**
 *  Optional callback: Called when a single QR / bar code is recognised.
 *  The supported standarts are:
 
 - UPC-A
 - UPC-E
 - EAN-8
 - EAN-13
 - Code 39
 - Code 93
 - Code 128
 - ITF
 - Codabar
 - RSS-14 (all variants)
 - QR Code
 - Data Matrix
 - Aztec ('beta' quality)
 - PDF 417 ('alpha' quality)
 
 */
@protocol QRMatchedProtocol <NSObject>

@optional
/**
 *  Called when a single QR / bar code is recognised.
 *
 *  @param res Code read string.
 */
-(void)singleQRMatchedResult:(NSString*)res ;

/**
 *  Called when multiple QR / bar codes are recognised.
 *
 *  @param codes Array of found ROIs. Each ROI has a CGRect defining the crop rectangle and the QR string response.
 */
-(void)multipleQRMatchedResult:(NSArray*)codes;

@end


/**
 * <b>Visual Search Plugin:</b><br>
 * This plugin analyzes the input video feed frames looking for matches within the images
 * pools previously inserted. When there is a positive match, the image information will
 * be returned.<br>
 * This plugin supports QR, bar code and regular image recognition.
 *
 */

__attribute__((__visibility__("default"))) @interface vsPlugin : UIViewController
{
    
}

/** @name Add Images */

/**
 * @brief Add an image to the system. The system will generate an automatic unique id.
 *
 * @param image The image to be added to the system.
 *
 * @return Image unique identifier, -1 if image is not added to the system (i.e. does not have enough features ).
 */
- (NSInteger) insertImage:(UIImage*)image vs_AVAILABLE(vs_V_0_1_1);

/**
 * @brief Add image to the system directly from URL. The system will generate an automatic unique id.
 *
 * @param imageUrl URL that holds the image to be added to the system.
 *
 * @return Image unique identifier, -1 if image is not added to the system (i.e. does not have enough features or has not been downloaded properly).
 */
- (NSInteger) insertImageFromURL:(NSURL*)imageUrl vs_AVAILABLE(vs_V_0_1_1);

/**
 * @brief Add image to the system. The user specifies the image unique id.
 *
 * @param image The image to be added to the system.
 *
 * @param uId The image identifier defined by the user. Must be unique.
 *
 *  @return YES if the image has been successfully inserted, NO otherwise (i.e. does not have enough features or the id is not unique).
 */
- (BOOL) insertImage:(UIImage*)image withId:(NSInteger)uId vs_AVAILABLE(vs_V_0_1_1);

/**
 * @brief Add image to the system directly from URL. The user specifies the image unique id.
 *
 * @param imageUrl URL that holds the image to be added to the system.
 *
 * @param uId The image identifier defined by the user. Must be unique.
 *
 * @return YES if the image has been successfully inserted, NO otherwise (i.e. does not have enough features,
 * has not been downloaded properly or the id is not unique).
 */
- (BOOL) insertImageFromURL:(NSURL*)imageUrl withId:(NSInteger)uId vs_AVAILABLE(vs_V_0_1_1);

/** @name Remove Images */

/**
 *  @brief Delete an image from the system.
 *
 *  @param uId Image unique id.
 *
 *  @return YES if the image has been properly deleted, NO otherwise.
 */
- (BOOL) deleteImage:(NSInteger)uId vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Remove all the images from the system.
 *
 *  @return YES if all images are properly deleted, NO otherwise.
 *
 */
- (BOOL) deleteAllImages vs_AVAILABLE(vs_V_0_1_1);



/** @name Process Images / Frames */

/**
 *  @brief Process an UIImage and look for positive matches against any image added to the system or scanned QR / bar codes.
 *
 *  @param image Input image to be analysed.
 *
 *  @param saveImageToPhotoAlbum Save Image to the device photo album after resize and crop. Set to YES this flag in order to check out the frame is being sent to the recognition process.
 */
- (void) processUIImage:(UIImage* )image saveImageToPhotoAlbum:(BOOL)saveImageToPhotoAlbum vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Process an image buffer and look for positive matches against any image added to the system or scanned QR / bar codes.
 *  Camera output settings should be either <b>kCVPixelFormatType_420YpCbCr8BiPlanarFullRange</b> or <b>kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange</b>
 *
 *  @param imageBuffer Image buffer input to be analysed.
 *
 *  @param saveImageToPhotoAlbum Save image to the device photo album after resize and crop. Set to YES this flag in order to check out the frame is being sent to the recognition process.
 */
- (void)processYUVFrame:(CVImageBufferRef)imageBuffer saveImageToPhotoAlbum:(BOOL)saveImageToPhotoAlbum vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Process an image buffer and look for positive matches against any image added to the system or scanned QR / bar codes.
 *  Camera output settings should be <b>kCVPixelFormatType_32BGRA</b>
 *
 *  @param imageBuffer Image buffer input to be analysed.
 *
 *  @param saveImageToPhotoAlbum Save image to the device photo album after resize and crop. Set to YES this flag in order to check out the frame is being sent to the recognition process.
 */
- (void)processRGBFrame:(CVImageBufferRef)imageBuffer saveImageToPhotoAlbum:(BOOL)saveImageToPhotoAlbum vs_AVAILABLE(vs_V_0_1_1);



/** @name Init and Config Params */

/**
 * @brief Init the visual search plugin
 *
 * @param key License Key
 *
 * @param isDebug Activate debug logs.
 */
- (id)initWithKey:(NSString *)key setDebug:(BOOL)isDebug vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Set the engine search mode: look for regular images, look for QR / bar codes or look for all.
 *
 *  @param smode Search_image (Look for regular images)<br>  Search_QRcodes (Look for QR / bar code)<br> Search_all (Both image and QR / bar code matching. (default option)).
 */
- (void) setSearchMode:(search_mode)smode vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief The images added to the system receive a score between 0 and 10 indicating
 *  how "rich" the image is (number of good features for matching). The closer to 10, the easier
 *  it is to match the image and more difficult is to get a false positive.
 *  Setting this param, the user decides the minimum quality of their pool of images.
 *
 *  @param score Score between (0-10). 0 - almost any image will be inserted, 10 - only the most feature-rich images will be inserted.
 *  <b>default threshold value is 5</b>
 */
- (void) setMatchingThreshold:(NSInteger)score vs_AVAILABLE(vs_V_0_1_1);

/**
 * @brief   Get the image quality.
 *
 * @param   image Input image to be analyzed.
 *
 * @return  Quality score.
 */
- (NSInteger)getImageScore:(UIImage*)image vs_AVAILABLE(vs_V_0_1_1);



/** @name QR / BAR Codes */

/**
 *  @brief Add a target rect or region of interest to look for QR / bar codes.
 *  If you define several rects, you will be able to read several QR / bar codes at a time.
 *
 *  @param frame The target region of interest.
 *
 *  @return YES if target rect successfully defined, NO otherwise (i.e. rect does not fit in the image or it overlaps other already defined rect).
 */
- (BOOL) addQRRect:(Roi*)frame vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Remove all target rects.
 *
 */
- (void) clearAllQRRects vs_AVAILABLE(vs_V_0_1_1);


/** @name Motion Filter */

/**
 *  @brief Enable Motion Detection filter: the engine will not search if the device is moving. This option makes it more difficult to get false positives.
 *
 *  @param threshold (0,10). 0 - very sensitive, 10 less sensitive. <b>(Default value 5)</b>
 *  @param debugLog Display a log message when the device is considered to be moving. This flag will let you decide what threshold to set according to your expectations.
 *
 */
- (void)initMotionDetectionWithThreshold:(NSInteger)threshold enableDebugLog:(BOOL)debugLog vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Disable Motion Detection filter.
 *
 */
- (void)removeMotionDetection vs_AVAILABLE(vs_V_0_1_1);



/** @name Properties */

/**
 *  @brief Crop rect: The system will crop the input image / frame and perform the search over that cropped area.
 *  The origin (0, 0) is considered the top-left screen corner.
 *  <b>Default (0 ,0 ,0 ,0) No cropping.</b>
 */
@property (assign, nonatomic) CGRect imageCropRect vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Frame Size: The input frame & added images height or width (max(w,h)) will be resized to this value. Higher sizes will improve the accuracy but slow the speed of matching.
 *  <b>Default 250px.</b>
 */
@property (assign, nonatomic) NSInteger frameSize vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Minimum number of features accepted to insert an image in the system.<br>
 *  The higher it is the most feature-rich images will be required to be inserted.<br>
 *  This value is related with the threshold: the higher the more restrictive the threshold will be.<br>
 *  <b>Default value is 50.</b>
 */
@property (assign, nonatomic, setter = setMinFeatures:, getter = getMinFeatures) NSInteger minFeatures vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Maximum number of features accepted to match an image.<br>
 *  When an image / frame is going to be matched against the images already inserted in the system, the engine
 *  extract the best features and compares them with the ones of the images in the system.
 *  The higher it is the more features will be accepted in the matching process.<br>
 *  The higher it is the better the matching quality will be, but the slower it will match.<br>
 *  <b>Default value is 50.</b>
 */
@property (assign, nonatomic, setter = setMaxFeatures:, getter = getMaxFeatures) NSInteger maxFeatures vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief The system uses internally a filter in order to assure the best matching result avoiding false positives.
 *  Higher sizes will improve the accuracy but slow the speed of matching.
 *  <b>Default value 5. Must be an odd number</b>
 */
@property (assign, nonatomic) NSInteger filterWindow vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Protocol delegate.
 */
@property (weak, nonatomic) id <imageMatchedProtocol> imageDelegate vs_AVAILABLE(vs_V_0_1_1);

/**
 *  @brief Protocol delegate.
 */
@property (weak, nonatomic) id <QRMatchedProtocol> qrDelegate vs_AVAILABLE(vs_V_0_1_1);


@end

#endif
