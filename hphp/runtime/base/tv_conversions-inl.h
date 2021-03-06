/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   | Copyright (c) 1998-2010 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/type_conversions.h" // toInt64(double)

namespace HPHP {

//////////////////////////////////////////////////////////////////////

inline bool cellToBool(Cell cell) {
  assert(cellIsPlausible(&cell));

  switch (cell.m_type) {
  case KindOfUninit:
  case KindOfNull:          return false;
  case KindOfInt64:         return cell.m_data.num != 0;
  case KindOfBoolean:       return cell.m_data.num;
  case KindOfDouble:        return cell.m_data.dbl != 0;
  case KindOfStaticString:
  case KindOfString:        return cell.m_data.pstr->toBoolean();
  case KindOfArray:         return !cell.m_data.parr->empty();
  case KindOfObject:        return cell.m_data.pobj->o_toBoolean();
  default:                  break;
  }
  not_reached();
}

inline int64_t cellToInt(Cell cell) {
  assert(cellIsPlausible(&cell));

  switch (cell.m_type) {
  case KindOfInt64:        return cell.m_data.num;
  case KindOfDouble:       return toInt64(cell.m_data.dbl);
  case KindOfString:
  case KindOfStaticString: return cell.m_data.pstr->toInt64(10);
  case KindOfArray:        return cell.m_data.parr->empty() ? 0 : 1;
  case KindOfObject:       return cell.m_data.pobj->o_toInt64();
  case KindOfBoolean:      return cell.m_data.num;
  case KindOfUninit:
  case KindOfNull:         return 0;
  default:                 break;
  }
  not_reached();
}

inline TypedNum stringToNumeric(const StringData* sd) {
  int64_t ival;
  double dval;
  auto const dt = sd->isNumericWithVal(ival, dval, true /* allow_errors */);
  return dt == KindOfInt64 ? make_tv<KindOfInt64>(ival) :
         dt == KindOfDouble ? make_tv<KindOfDouble>(dval) :
         make_tv<KindOfInt64>(0);
}

//////////////////////////////////////////////////////////////////////

}
