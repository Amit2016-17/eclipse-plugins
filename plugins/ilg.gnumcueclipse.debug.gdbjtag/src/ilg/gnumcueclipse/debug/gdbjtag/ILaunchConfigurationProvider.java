/*******************************************************************************
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 * 
 * Contributors:
 *     Liviu Ionescu - initial version
 *******************************************************************************/

package ilg.gnumcueclipse.debug.gdbjtag;

import org.eclipse.debug.core.ILaunchConfiguration;

public interface ILaunchConfigurationProvider {

	// ------------------------------------------------------------------------

	public ILaunchConfiguration getLaunchConfiguration();

	// ------------------------------------------------------------------------
}