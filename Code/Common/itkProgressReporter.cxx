/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkProgressReporter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"

namespace itk
{

//----------------------------------------------------------------------------
ProgressReporter::ProgressReporter(ProcessObject* filter, int threadId,
                                   unsigned long numberOfPixels,
                                   unsigned long numberOfUpdates,
                                   float initialProgress,
                                   float progressWeight):
  m_Filter(filter),
  m_ThreadId(threadId),
  m_CurrentPixel(0),
  m_InitialProgress( initialProgress ),
  m_ProgressWeight( progressWeight )
{
  // Only thread 0 should update progress.
  if(m_ThreadId == 0)
    {

    float numPixels = numberOfPixels;
    float numUpdates = numberOfUpdates;
    
    // Make sure we have at least one pixel.
    if(numPixels < 1)
      {
      numPixels = 1;
      }
    
    // We cannot update more times than there are pixels.
    if(numUpdates > numPixels)
      {
      numUpdates = numPixels;
      }
    
    // Calculate the interval for updates.
    m_PixelsPerUpdate = static_cast<unsigned long>(numPixels/numUpdates);
    m_InverseNumberOfPixels = 1.0 / numPixels;
    
    // Set the progress to initial progress.  The filter is just starting.
    m_Filter->UpdateProgress( m_InitialProgress );
    }
  else
    {
    // Threads other than 0 should never report progress.
    m_PixelsPerUpdate = NumericTraits<unsigned long>::max();
    m_InverseNumberOfPixels = 0;
    }
  m_PixelsBeforeUpdate = m_PixelsPerUpdate;
}

//----------------------------------------------------------------------------
ProgressReporter::~ProgressReporter()
{
  // Only thread 0 should update progress.
  if(m_ThreadId == 0)
    {
    // Set the progress to the end of its current range.  The filter has finished.
    m_Filter->UpdateProgress( m_InitialProgress + m_ProgressWeight );
    }
}  

} // end namespace itk
