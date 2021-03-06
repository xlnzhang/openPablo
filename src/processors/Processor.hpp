/*
 *  Processor.hpp
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


#ifndef OPENPABLO_PROCESSOR_H_
#define OPENPABLO_PROCESSOR_H_

/*
 * @mainpage Processor
 *
 * Description in html
 * @author Aydin Demircioglu
  */


/*
 * @file Processor.hpp
 *
 * @brief description in brief.
 *
 */


#include <QString>
#include <stdint.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/string_path.hpp>
#include <boost/property_tree/json_parser.hpp>


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
    class Processor
    {
        public:
            /*
             *
             */
            void setFilename (QString _filename);

            void setEngine (QString _engineID);

            void setSettings (boost::property_tree::ptree _pt);

            virtual void setBLOB (unsigned char *data, uint64_t datalength) = 0;

            virtual void start() = 0;

            virtual ~Processor();

        protected:

            boost::property_tree::ptree pt;

            QString filename;

            QString engineID;
    };

}


#endif // OPENPABLO_PROCESSOR_H_
