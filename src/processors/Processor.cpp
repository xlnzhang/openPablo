/*
 *  Processor.cpp
 *
 *
 *  This file is part of openPablo.
 *
 *  Copyright (c) 2012- Aydin Demircioglu (aydin@openpablo.org)
 *
 *  openPablo is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  openPablo is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with openPablo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Processor.hpp"


/*
 * @mainpage Processor
 *
 * Description in html
 * @author Aydin Demircioglu
  */


/*
 * @file Processor.cpp
 *
 * @brief description in brief.
 *
 */



namespace openPablo
{

    /*
     * @class Processor
     *
     * @brief Abstract class to interface the capabilities of a processor
     *
     * Abstract class..
     *
     */

    void Processor::setFilename (QString _filename)
    {
        filename = _filename;
    }



    void Processor::setEngine (QString _engineID)
    {
        engineID = _engineID;
    }



    void Processor::setSettings (boost::property_tree::ptree _pt)
    {
        pt = _pt;
    }



    Processor::~Processor()
    {
        //
    }


}
