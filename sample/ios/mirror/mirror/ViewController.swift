 //
//  ViewController.swift
//  maskshow
//
//  Created by buffer on 16/1/21.
//  Copyright © 2016年 xxx. All rights reserved.
//

import UIKit
import GLKit

class ViewController: GLKViewController, CameraHelperDelegate {
  let helperCam = CameraHelper.shareCameraHelper();
  
  var rotImageBuffer:Array<UInt8>!;
  var imageWidth:Int = 0;
  var imageHeight:Int = 0;
  var imagewidstep:Int = 0;

//------------------------------------------------------------------------------------------------------------------
  override func viewDidLoad() {
    super.viewDidLoad()
    
    // setting render openGL context.
    let context = EAGLContext(API: .OpenGLES2) ;
    //EAGLContext.setCurrentContext(context);
    let view = self.view as! GLKView
    view.context = context
    view.drawableDepthFormat = .Format24
    
    //Fist, init AVCaptrue and start is necessary.
    helperCam.setVideoDataOutputBuffer(self);
    helperCam.startVideoCapture();
    
    //Second, Add preVPlayer in self view layer.
    //let prevPlayer = helperCam.getAVCaptureVideoPreviewLayer();
    //prevPlayer.frame = CGRectMake(self.view.frame.width-100, 0, 100, 100);
    //prevPlayer.borderWidth = 2;
    //prevPlayer.borderColor = UIColor.greenColor().CGColor;
    //self.view.layer.addSublayer(prevPlayer);
  }
  
  deinit {
  }

  override func didReceiveMemoryWarning() {
    super.didReceiveMemoryWarning()
    // Dispose of any resources that can be recreated.
  }

//------------------------------------------------------------------------------------------------------------------
  
  //delegate
  override func glkView(view: GLKView, drawInRect rect: CGRect) {
    if self.rotImageBuffer == nil {
      return
    }
    
    glClearColor(0.58,0.73,0.89,0);
    glClear(GLbitfield(GL_COLOR_BUFFER_BIT) | GLbitfield(GL_DEPTH_BUFFER_BIT));
    
    objc_sync_enter(self)
    
    let id = unsafeAddressOf(view)
    let native_windows_id = UInt(id.hashValue)
    let width = UInt32(imageWidth)
    let height = UInt32(imageHeight);
    GBCaller.image_draw(
      UnsafeMutablePointer<UInt8>(rotImageBuffer), width: width, height: height, windows: native_windows_id)
    objc_sync_exit(self)
  }
  
  @objc func VideoDataOutputBuffer(sampleBuffer: CMSampleBuffer!) {
    objc_sync_enter(self)
    let imageBuffer:CVPixelBufferRef! = CMSampleBufferGetImageBuffer(sampleBuffer);
    if  CVPixelBufferLockBaseAddress(imageBuffer, 0)  == kCVReturnSuccess
    {
      let baseAddress = CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
      imageWidth = CVPixelBufferGetWidth(imageBuffer);
      imageHeight = CVPixelBufferGetHeight(imageBuffer);
      imagewidstep = CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0);
      //Rotation the image buffer.8
      if self.rotImageBuffer == nil {
        self.rotImageBuffer = Array(count: Int(imagewidstep*imageHeight*3/2), repeatedValue: 0);
      }
      //helperCam.rotateImageInYPlane(UnsafeMutablePointer<UInt8>(baseAddress), withWidth: Int32(imageWidth),
      //  withHeight: Int32(imageHeight), destBuffer:&self.rotImageBuffer!);
      //let newWidth = (imageWidth);
      //imageWidth = (imageHeight);
      //imageHeight = (newWidth);
      //imagewidstep = imageWidth;
      
      helperCam.NV21toI420(UnsafeMutablePointer<UInt8>(baseAddress), withWidth: Int32(imagewidstep),
        withHeight: Int32(imageHeight), destBuffer:&self.rotImageBuffer!);
    }
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
    objc_sync_exit(self)
  }
}
