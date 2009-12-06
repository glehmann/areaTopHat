/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWhiteTopHatByAreaImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-28 14:36:34 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWhiteTopHatByAreaImageFilter_h
#define __itkWhiteTopHatByAreaImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class WhiteTopHatByAreaImageFilter
 * \brief White top hat extract regional maxima that are bigger than the lambda parameter
 *
 * If the filter is used with UseImageSpacingOff, it may be useful to give an
 * integer type as attribute type - generally, unsigned long.
 * 
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TAttribute=ITK_TYPENAME TInputImage::SpacingType::ValueType>
class ITK_EXPORT WhiteTopHatByAreaImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef WhiteTopHatByAreaImageFilter                  Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;

  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
  typedef TAttribute                               AttributeType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(WhiteTopHatByAreaImageFilter,
               ImageToImageFilter);

  /**
   * Set/Get whether the image spacing is used or not - defaults to true.
   */
  itkSetMacro(UseImageSpacing, bool);
  itkGetConstReferenceMacro(UseImageSpacing, bool);
  itkBooleanMacro(UseImageSpacing);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);

  /**
   * Set/Get the threshold value used to select the connected components.
   * The connected components greater or equal to Lambda are kept, the others
   * are removed. Lambda defaults to 0.
   */
  itkSetMacro(Lambda, AttributeType);
  itkGetConstMacro(Lambda, AttributeType);

protected:
  WhiteTopHatByAreaImageFilter();
  ~WhiteTopHatByAreaImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** WhiteTopHatByAreaImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  void GenerateData();
  

private:
  WhiteTopHatByAreaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool          m_UseImageSpacing;
  bool          m_FullyConnected;
  AttributeType m_Lambda;
  
}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWhiteTopHatByAreaImageFilter.txx"
#endif

#endif
