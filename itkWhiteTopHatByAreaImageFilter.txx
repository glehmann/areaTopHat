/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWhiteTopHatByAreaImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-08 16:03:55 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWhiteTopHatByAreaImageFilter_txx
#define __itkWhiteTopHatByAreaImageFilter_txx

#include "itkWhiteTopHatByAreaImageFilter.h"
#include "itkAreaOpeningImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template <class TInputImage, class TOutputImage, class TAttribute>
WhiteTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::WhiteTopHatByAreaImageFilter()
{
    m_UseImageSpacing = true;
    m_FullyConnected = false;
    m_Lambda = 0;
}

template <class TInputImage, class TOutputImage, class TAttribute>
void 
WhiteTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TAttribute>
void 
WhiteTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TAttribute>
void 
WhiteTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  // Delegate to an opening filter.
  typedef AreaOpeningImageFilter<InputImageType, InputImageType, AttributeType> OpeningType;
  typename OpeningType::Pointer open = OpeningType::New();
  open->SetInput( this->GetInput() );
  open->SetLambda( m_Lambda );
  open->SetFullyConnected( m_FullyConnected );
  open->SetUseImageSpacing( m_UseImageSpacing );
  
  // Need to subtract the opened image from the input
  typedef SubtractImageFilter<TInputImage, TInputImage, TOutputImage> SubtractType;
  typename SubtractType::Pointer subtract = SubtractType::New();
  subtract->SetInput1( this->GetInput() );
  subtract->SetInput2( open->GetOutput() );

  // graft our output to the subtract filter to force the proper regions
  // to be generated
  subtract->GraftOutput( this->GetOutput() );

  // run the algorithm
  progress->RegisterInternalFilter(open,.9f);
  progress->RegisterInternalFilter(subtract,.1f);

  subtract->Update();

  // graft the output of the subtract filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( subtract->GetOutput() );

}

template<class TInputImage, class TOutputImage, class TAttribute>
void
WhiteTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Lambda: "  << static_cast< typename NumericTraits< AttributeType >::PrintType>( m_Lambda ) << std::endl;
  os << indent << "FullyConnected: " << m_FullyConnected << std::endl;
  os << indent << "UseImageSpacing: " << m_UseImageSpacing << std::endl;
}

}// end namespace itk
#endif
