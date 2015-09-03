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
 * File: vsMotion.h
 * Description:
 * Author: Pablo GM (info@aumentia.com)
 * Created: 01/09/15.
 * Verion 0.5
 *
 *
 **************************************************************************/

#import "_vs_versions.h"

@protocol vsMotionProtocol <NSObject>

@optional

/**
 *  Optional protocol called when a button is clicked.
 *  @param buttonId Button unique identifier
 */
-(void)buttonClicked:(NSNumber*)buttonId;

/**
 *  Optional protocol When a button is clicked all the rest remain inactive for some frames.
 *                    This function let us know if the buttons are active or inactive.
 *  @param isActive YES if all the buttons are active, NO otherwise
 */
-(void)buttonsActive:(BOOL)isActive;

@end

/**
 * Default values to disable the buttons once one is clicked
 *
 */
typedef NS_ENUM(NSInteger, vsMotionDelay)
{
    /**
     * Delay of 5 frames
     */
    LOWDELAY        = 5,
    /**
     * Delay of 10 frames
     */
    MEDIUMDELAY     = 10,
    /**
     * Delay of 25 frames
     */
    HIGHDELAY       = 25,
};

/**
 * <b>Motion Detection Plugin:</b><br>
 * This plugin analyzes the input video feed frames trying to detect motion 
 * in predefined regions of interest (ROIs), what we called, virtual buttons.
 *
 */

__attribute__((__visibility__("default"))) @interface vsMotion : NSObject


/** @name Properties */

/**
 *  Protocol delegate.
 */
@property (weak, nonatomic) id <vsMotionProtocol> vsMotionDelegate;


/** @name Init and Config */

/**
 * @brief   Init motion detection instance.
 *
 * @param   key License Key
 * @param   isDebug Activate debug logs.
 *
 * @return  Quality Rating
 */
-(id)initWithKey:(NSString *)key setDebug:(BOOL)isDebug vs_AVAILABLE(vs_V_0_5);

/**
 * @brief   Set number of frames the virtual buttons will remain inactive after one is triggered.
 *
 * @param   period Number of frames.
 *
 * @discussion  The inactive time will change depending on the device framerate.
 * See default values @link vsMotionDelay
 */
-(void)setInactivePeriod:(NSNumber*)period vs_AVAILABLE(vs_V_0_5);


/** @name Add virtual buttons */

/**
 * @brief   Add Virtual Button. The system will generate a unique Id.
 *
 * @param   rect  Rectangle values are represented in percentage (values from 0 to 100).
 *                Example: CGRect(10, 10, 25, 50):
 *                x = 0.1*image_width, y = 0.1*image_height, w = 0.25*image_width and h = 0.50 * image_height
 *
 * @return  Button unique Id.
 */
-(NSNumber *)addButtonWithRect:(CGRect)rect vs_AVAILABLE(vs_V_0_5);

/**
 * @brief   Add Virtual Button with unique Id.
 *
 * @param   rect  Rectangle values are represented in percentage (values from 0 to 100).
 *                Example: CGRect(10, 10, 25, 50):
 *                x = 0.1*image_width, y = 0.1*image_height, w = 0.25*image_width and h = 0.50 * image_height
 * @param   uId Unique button Id.
 *
 * @return  YES if the button has been properly added, NO otherwise.
 */
-(BOOL)addButtonWithRect:(CGRect)rect withUniqeID:(NSNumber*)uId vs_AVAILABLE(vs_V_0_5);


/** @name Remove virtual buttons */

/**
 * @brief   Remove Virtual Button with unique Id.
 *
 * @param   uId Unique button Id.
 *
 * @return  YES if the button has been properly removed, NO otherwise.
 */
-(BOOL)removeButtonWithId:(NSNumber*)uId vs_AVAILABLE(vs_V_0_5);

/**
 * @brief   Remove all virtual buttons.
 *
 * @return  YES if the buttons have properly been removed.
 */
-(BOOL)clearButtons vs_AVAILABLE(vs_V_0_5);


/** @name Motion Filter */

/**
 *  @brief Enable Motion Detection filter: the engine will not search if the device is moving. This option makes it more difficult to get false positives.
 *
 *  @param threshold (0,10). 0 - very sensitive, 10 less sensitive. <b>(Default value 5)</b>
 *  @param debugLog Display a log message when the device is considered to be moving. This flag will let you decide what threshold to set according to your expectations.
 *
 */
- (void)initMotionDetectionWithThreshold:(NSInteger)threshold enableDebugLog:(BOOL)debugLog vs_AVAILABLE(vs_V_0_5);

/**
 *  @brief Disable Motion Detection filter.
 *
 */
- (void)removeMotionDetection vs_AVAILABLE(vs_V_0_5);


/** @name Process Frames */

/**
 *  @brief Process an image buffer and detect motion.
 *  Camera output settings should be <b>kCVPixelFormatType_32BGRA</b>
 *
 *  @param imageBuffer Image buffer input to be analysed.
 *
 *  @param saveImageToPhotoAlbum Save image, with the virtual buttons areas drawn over it, to the device photo album. Set to YES this flag in order to check out the frame is being sent to the motion detection process.
 */
- (void)processRGBFrame:(CVImageBufferRef)imageBuffer saveImageToPhotoAlbum:(BOOL)saveImageToPhotoAlbum vs_AVAILABLE(vs_V_0_5);

/**
 *  @brief Process an image buffer and detect motion
 *  Camera output settings should be either <b>kCVPixelFormatType_420YpCbCr8BiPlanarFullRange</b> or <b>kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange</b>
 *
 *  @param imageBuffer Image buffer input to be analysed.
 *
 *  @param saveImageToPhotoAlbum Save image, with the virtual buttons areas drawn over it, to the device photo album. Set to YES this flag in order to check out the frame is being sent to the motion detection process.
 */
- (void)processYUVFrame:(CVImageBufferRef)imageBuffer saveImageToPhotoAlbum:(BOOL)saveImageToPhotoAlbum vs_AVAILABLE(vs_V_0_5);

/**
 *  @brief Process an image buffer and detect motion
 *
 *  @param image Input image to be analysed.
 *
 *  @param saveImageToPhotoAlbum Save image, with the virtual buttons areas drawn over it, to the device photo album. Set to YES this flag in order to check out the frame is being sent to the motion detection process.
 */
- (void)processUIImage:(UIImage* )image saveImageToPhotoAlbum:(BOOL)saveImageToPhotoAlbum vs_AVAILABLE(vs_V_0_5);

@end