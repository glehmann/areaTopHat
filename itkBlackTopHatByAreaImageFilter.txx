/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBlackTopHatByAreaImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-08 16:03:55 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBlackTopHatByAreaImageFilter_txx
#define __itkBlackTopHatByAreaImageFilter_txx

#include "itkBlackTopHatByAreaImageFilter.h"
#include "itkAreaClosingImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template <class TInputImage, class TOutputImage, class TAttribute>
BlackTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::BlackTopHatByAreaImageFilter()
{
  m_UseImageSpacing = true;
  m_FullyConnected = false;
  m_Lambda = 0;
}

template <class TInputImage, class TOutputImage, class TAttribute>
void 
BlackTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
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
BlackTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  // Delegate to an closeing filter.
  typedef AreaClosingImageFilter<InputImageType, InputImageType, AttributeType> ClosingType;
  typename ClosingType::Pointer close = ClosingType::New();
  close->SetInput( this->GetInput() );
  close->SetLambda( m_Lambda );
  close->SetFullyConnected( m_FullyConnected );
  close->SetUseImageSpacing( m_UseImageSpacing );
  
  // Need to subtract the closeed image from the input
  typedef SubtractImageFilter<TInputImage, TInputImage, TOutputImage> SubtractType;
  typename SubtractType::Pointer subtract = SubtractType::New();
  subtract->SetInput1( close->GetOutput() );
  subtract->SetInput2( this->GetInput() );

  // graft our output to the subtract filter to force the proper regions
  // to be generated
  subtract->GraftOutput( this->GetOutput() );

  // run the algorithm
  progress->RegisterInternalFilter(close,.9f);
  progress->RegisterInternalFilter(subtract,.1f);

  subtract->Update();

  // graft the output of the subtract filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( subtract->GetOutput() );

}

template<class TInputImage, class TOutputImage, class TAttribute>
void
BlackTopHatByAreaImageFilter<TInputImage, TOutputImage, TAttribute>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Lambda: "  << static_cast< typename NumericTraits< AttributeType >::PrintType>( m_Lambda ) << std::endl;
  os << indent << "FullyConnected: " << m_FullyConnected << std::endl;
  os << indent << "UseImageSpacing: " << m_UseImageSpacing << std::endl;
}

}// end namespace itk
#endif
