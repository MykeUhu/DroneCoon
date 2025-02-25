﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cloudformation/CloudFormation_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/cloudformation/model/StackInstanceStatus.h>
#include <aws/cloudformation/model/StackInstanceComprehensiveStatus.h>
#include <aws/cloudformation/model/StackDriftStatus.h>
#include <aws/core/utils/DateTime.h>
#include <aws/cloudformation/model/Parameter.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Xml
{
  class XmlNode;
} // namespace Xml
} // namespace Utils
namespace CloudFormation
{
namespace Model
{

  /**
   * <p>A CloudFormation stack, in a specific account and Region, that's part of a
   * stack set operation. A stack instance is a reference to an attempted or actual
   * stack in a given account within a given Region. A stack instance can exist
   * without a stack—for example, if the stack couldn't be created for some reason. A
   * stack instance is associated with only one stack set. Each stack instance
   * contains the ID of its associated stack set, in addition to the ID of the actual
   * stack and the stack status.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/cloudformation-2010-05-15/StackInstance">AWS
   * API Reference</a></p>
   */
  class StackInstance
  {
  public:
    AWS_CLOUDFORMATION_API StackInstance();
    AWS_CLOUDFORMATION_API StackInstance(const Aws::Utils::Xml::XmlNode& xmlNode);
    AWS_CLOUDFORMATION_API StackInstance& operator=(const Aws::Utils::Xml::XmlNode& xmlNode);

    AWS_CLOUDFORMATION_API void OutputToStream(Aws::OStream& ostream, const char* location, unsigned index, const char* locationValue) const;
    AWS_CLOUDFORMATION_API void OutputToStream(Aws::OStream& oStream, const char* location) const;


    ///@{
    /**
     * <p>The name or unique ID of the stack set that the stack instance is associated
     * with.</p>
     */
    inline const Aws::String& GetStackSetId() const{ return m_stackSetId; }
    inline bool StackSetIdHasBeenSet() const { return m_stackSetIdHasBeenSet; }
    inline void SetStackSetId(const Aws::String& value) { m_stackSetIdHasBeenSet = true; m_stackSetId = value; }
    inline void SetStackSetId(Aws::String&& value) { m_stackSetIdHasBeenSet = true; m_stackSetId = std::move(value); }
    inline void SetStackSetId(const char* value) { m_stackSetIdHasBeenSet = true; m_stackSetId.assign(value); }
    inline StackInstance& WithStackSetId(const Aws::String& value) { SetStackSetId(value); return *this;}
    inline StackInstance& WithStackSetId(Aws::String&& value) { SetStackSetId(std::move(value)); return *this;}
    inline StackInstance& WithStackSetId(const char* value) { SetStackSetId(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>The name of the Amazon Web Services Region that the stack instance is
     * associated with.</p>
     */
    inline const Aws::String& GetRegion() const{ return m_region; }
    inline bool RegionHasBeenSet() const { return m_regionHasBeenSet; }
    inline void SetRegion(const Aws::String& value) { m_regionHasBeenSet = true; m_region = value; }
    inline void SetRegion(Aws::String&& value) { m_regionHasBeenSet = true; m_region = std::move(value); }
    inline void SetRegion(const char* value) { m_regionHasBeenSet = true; m_region.assign(value); }
    inline StackInstance& WithRegion(const Aws::String& value) { SetRegion(value); return *this;}
    inline StackInstance& WithRegion(Aws::String&& value) { SetRegion(std::move(value)); return *this;}
    inline StackInstance& WithRegion(const char* value) { SetRegion(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>[Self-managed permissions] The name of the Amazon Web Services account that
     * the stack instance is associated with.</p>
     */
    inline const Aws::String& GetAccount() const{ return m_account; }
    inline bool AccountHasBeenSet() const { return m_accountHasBeenSet; }
    inline void SetAccount(const Aws::String& value) { m_accountHasBeenSet = true; m_account = value; }
    inline void SetAccount(Aws::String&& value) { m_accountHasBeenSet = true; m_account = std::move(value); }
    inline void SetAccount(const char* value) { m_accountHasBeenSet = true; m_account.assign(value); }
    inline StackInstance& WithAccount(const Aws::String& value) { SetAccount(value); return *this;}
    inline StackInstance& WithAccount(Aws::String&& value) { SetAccount(std::move(value)); return *this;}
    inline StackInstance& WithAccount(const char* value) { SetAccount(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>The ID of the stack instance.</p>
     */
    inline const Aws::String& GetStackId() const{ return m_stackId; }
    inline bool StackIdHasBeenSet() const { return m_stackIdHasBeenSet; }
    inline void SetStackId(const Aws::String& value) { m_stackIdHasBeenSet = true; m_stackId = value; }
    inline void SetStackId(Aws::String&& value) { m_stackIdHasBeenSet = true; m_stackId = std::move(value); }
    inline void SetStackId(const char* value) { m_stackIdHasBeenSet = true; m_stackId.assign(value); }
    inline StackInstance& WithStackId(const Aws::String& value) { SetStackId(value); return *this;}
    inline StackInstance& WithStackId(Aws::String&& value) { SetStackId(std::move(value)); return *this;}
    inline StackInstance& WithStackId(const char* value) { SetStackId(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>A list of parameters from the stack set template whose values have been
     * overridden in this stack instance.</p>
     */
    inline const Aws::Vector<Parameter>& GetParameterOverrides() const{ return m_parameterOverrides; }
    inline bool ParameterOverridesHasBeenSet() const { return m_parameterOverridesHasBeenSet; }
    inline void SetParameterOverrides(const Aws::Vector<Parameter>& value) { m_parameterOverridesHasBeenSet = true; m_parameterOverrides = value; }
    inline void SetParameterOverrides(Aws::Vector<Parameter>&& value) { m_parameterOverridesHasBeenSet = true; m_parameterOverrides = std::move(value); }
    inline StackInstance& WithParameterOverrides(const Aws::Vector<Parameter>& value) { SetParameterOverrides(value); return *this;}
    inline StackInstance& WithParameterOverrides(Aws::Vector<Parameter>&& value) { SetParameterOverrides(std::move(value)); return *this;}
    inline StackInstance& AddParameterOverrides(const Parameter& value) { m_parameterOverridesHasBeenSet = true; m_parameterOverrides.push_back(value); return *this; }
    inline StackInstance& AddParameterOverrides(Parameter&& value) { m_parameterOverridesHasBeenSet = true; m_parameterOverrides.push_back(std::move(value)); return *this; }
    ///@}

    ///@{
    /**
     * <p>The status of the stack instance, in terms of its synchronization with its
     * associated stack set.</p> <ul> <li> <p> <code>INOPERABLE</code>: A
     * <code>DeleteStackInstances</code> operation has failed and left the stack in an
     * unstable state. Stacks in this state are excluded from further
     * <code>UpdateStackSet</code> operations. You might need to perform a
     * <code>DeleteStackInstances</code> operation, with <code>RetainStacks</code> set
     * to <code>true</code>, to delete the stack instance, and then delete the stack
     * manually. <code>INOPERABLE</code> can be returned here when the cause is a
     * failed import. If it's due to a failed import, the operation can be retried once
     * the failures are fixed. To see if this is due to a failed import, look at the
     * <code>DetailedStatus</code> member in the <code>StackInstanceSummary</code>
     * member that is a peer to this <code>Status</code> member.</p> </li> <li> <p>
     * <code>OUTDATED</code>: The stack isn't currently up to date with the stack set
     * because:</p> <ul> <li> <p>The associated stack failed during a
     * <code>CreateStackSet</code> or <code>UpdateStackSet</code> operation.</p> </li>
     * <li> <p>The stack was part of a <code>CreateStackSet</code> or
     * <code>UpdateStackSet</code> operation that failed or was stopped before the
     * stack was created or updated.</p> </li> </ul> </li> <li> <p>
     * <code>CURRENT</code>: The stack is currently up to date with the stack set.</p>
     * </li> </ul>
     */
    inline const StackInstanceStatus& GetStatus() const{ return m_status; }
    inline bool StatusHasBeenSet() const { return m_statusHasBeenSet; }
    inline void SetStatus(const StackInstanceStatus& value) { m_statusHasBeenSet = true; m_status = value; }
    inline void SetStatus(StackInstanceStatus&& value) { m_statusHasBeenSet = true; m_status = std::move(value); }
    inline StackInstance& WithStatus(const StackInstanceStatus& value) { SetStatus(value); return *this;}
    inline StackInstance& WithStatus(StackInstanceStatus&& value) { SetStatus(std::move(value)); return *this;}
    ///@}

    ///@{
    /**
     * <p>The detailed status of the stack instance.</p>
     */
    inline const StackInstanceComprehensiveStatus& GetStackInstanceStatus() const{ return m_stackInstanceStatus; }
    inline bool StackInstanceStatusHasBeenSet() const { return m_stackInstanceStatusHasBeenSet; }
    inline void SetStackInstanceStatus(const StackInstanceComprehensiveStatus& value) { m_stackInstanceStatusHasBeenSet = true; m_stackInstanceStatus = value; }
    inline void SetStackInstanceStatus(StackInstanceComprehensiveStatus&& value) { m_stackInstanceStatusHasBeenSet = true; m_stackInstanceStatus = std::move(value); }
    inline StackInstance& WithStackInstanceStatus(const StackInstanceComprehensiveStatus& value) { SetStackInstanceStatus(value); return *this;}
    inline StackInstance& WithStackInstanceStatus(StackInstanceComprehensiveStatus&& value) { SetStackInstanceStatus(std::move(value)); return *this;}
    ///@}

    ///@{
    /**
     * <p>The explanation for the specific status code that's assigned to this stack
     * instance.</p>
     */
    inline const Aws::String& GetStatusReason() const{ return m_statusReason; }
    inline bool StatusReasonHasBeenSet() const { return m_statusReasonHasBeenSet; }
    inline void SetStatusReason(const Aws::String& value) { m_statusReasonHasBeenSet = true; m_statusReason = value; }
    inline void SetStatusReason(Aws::String&& value) { m_statusReasonHasBeenSet = true; m_statusReason = std::move(value); }
    inline void SetStatusReason(const char* value) { m_statusReasonHasBeenSet = true; m_statusReason.assign(value); }
    inline StackInstance& WithStatusReason(const Aws::String& value) { SetStatusReason(value); return *this;}
    inline StackInstance& WithStatusReason(Aws::String&& value) { SetStatusReason(std::move(value)); return *this;}
    inline StackInstance& WithStatusReason(const char* value) { SetStatusReason(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>[Service-managed permissions] The organization root ID or organizational unit
     * (OU) IDs that you specified for <a
     * href="https://docs.aws.amazon.com/AWSCloudFormation/latest/APIReference/API_DeploymentTargets.html">DeploymentTargets</a>.</p>
     */
    inline const Aws::String& GetOrganizationalUnitId() const{ return m_organizationalUnitId; }
    inline bool OrganizationalUnitIdHasBeenSet() const { return m_organizationalUnitIdHasBeenSet; }
    inline void SetOrganizationalUnitId(const Aws::String& value) { m_organizationalUnitIdHasBeenSet = true; m_organizationalUnitId = value; }
    inline void SetOrganizationalUnitId(Aws::String&& value) { m_organizationalUnitIdHasBeenSet = true; m_organizationalUnitId = std::move(value); }
    inline void SetOrganizationalUnitId(const char* value) { m_organizationalUnitIdHasBeenSet = true; m_organizationalUnitId.assign(value); }
    inline StackInstance& WithOrganizationalUnitId(const Aws::String& value) { SetOrganizationalUnitId(value); return *this;}
    inline StackInstance& WithOrganizationalUnitId(Aws::String&& value) { SetOrganizationalUnitId(std::move(value)); return *this;}
    inline StackInstance& WithOrganizationalUnitId(const char* value) { SetOrganizationalUnitId(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>Status of the stack instance's actual configuration compared to the expected
     * template and parameter configuration of the stack set to which it belongs.</p>
     * <ul> <li> <p> <code>DRIFTED</code>: The stack differs from the expected template
     * and parameter configuration of the stack set to which it belongs. A stack
     * instance is considered to have drifted if one or more of the resources in the
     * associated stack have drifted.</p> </li> <li> <p> <code>NOT_CHECKED</code>:
     * CloudFormation hasn't checked if the stack instance differs from its expected
     * stack set configuration.</p> </li> <li> <p> <code>IN_SYNC</code>: The stack
     * instance's actual configuration matches its expected stack set
     * configuration.</p> </li> <li> <p> <code>UNKNOWN</code>: This value is reserved
     * for future use.</p> </li> </ul>
     */
    inline const StackDriftStatus& GetDriftStatus() const{ return m_driftStatus; }
    inline bool DriftStatusHasBeenSet() const { return m_driftStatusHasBeenSet; }
    inline void SetDriftStatus(const StackDriftStatus& value) { m_driftStatusHasBeenSet = true; m_driftStatus = value; }
    inline void SetDriftStatus(StackDriftStatus&& value) { m_driftStatusHasBeenSet = true; m_driftStatus = std::move(value); }
    inline StackInstance& WithDriftStatus(const StackDriftStatus& value) { SetDriftStatus(value); return *this;}
    inline StackInstance& WithDriftStatus(StackDriftStatus&& value) { SetDriftStatus(std::move(value)); return *this;}
    ///@}

    ///@{
    /**
     * <p>Most recent time when CloudFormation performed a drift detection operation on
     * the stack instance. This value will be <code>NULL</code> for any stack instance
     * on which drift detection hasn't yet been performed.</p>
     */
    inline const Aws::Utils::DateTime& GetLastDriftCheckTimestamp() const{ return m_lastDriftCheckTimestamp; }
    inline bool LastDriftCheckTimestampHasBeenSet() const { return m_lastDriftCheckTimestampHasBeenSet; }
    inline void SetLastDriftCheckTimestamp(const Aws::Utils::DateTime& value) { m_lastDriftCheckTimestampHasBeenSet = true; m_lastDriftCheckTimestamp = value; }
    inline void SetLastDriftCheckTimestamp(Aws::Utils::DateTime&& value) { m_lastDriftCheckTimestampHasBeenSet = true; m_lastDriftCheckTimestamp = std::move(value); }
    inline StackInstance& WithLastDriftCheckTimestamp(const Aws::Utils::DateTime& value) { SetLastDriftCheckTimestamp(value); return *this;}
    inline StackInstance& WithLastDriftCheckTimestamp(Aws::Utils::DateTime&& value) { SetLastDriftCheckTimestamp(std::move(value)); return *this;}
    ///@}

    ///@{
    /**
     * <p>The last unique ID of a StackSet operation performed on a stack instance.</p>
     */
    inline const Aws::String& GetLastOperationId() const{ return m_lastOperationId; }
    inline bool LastOperationIdHasBeenSet() const { return m_lastOperationIdHasBeenSet; }
    inline void SetLastOperationId(const Aws::String& value) { m_lastOperationIdHasBeenSet = true; m_lastOperationId = value; }
    inline void SetLastOperationId(Aws::String&& value) { m_lastOperationIdHasBeenSet = true; m_lastOperationId = std::move(value); }
    inline void SetLastOperationId(const char* value) { m_lastOperationIdHasBeenSet = true; m_lastOperationId.assign(value); }
    inline StackInstance& WithLastOperationId(const Aws::String& value) { SetLastOperationId(value); return *this;}
    inline StackInstance& WithLastOperationId(Aws::String&& value) { SetLastOperationId(std::move(value)); return *this;}
    inline StackInstance& WithLastOperationId(const char* value) { SetLastOperationId(value); return *this;}
    ///@}
  private:

    Aws::String m_stackSetId;
    bool m_stackSetIdHasBeenSet = false;

    Aws::String m_region;
    bool m_regionHasBeenSet = false;

    Aws::String m_account;
    bool m_accountHasBeenSet = false;

    Aws::String m_stackId;
    bool m_stackIdHasBeenSet = false;

    Aws::Vector<Parameter> m_parameterOverrides;
    bool m_parameterOverridesHasBeenSet = false;

    StackInstanceStatus m_status;
    bool m_statusHasBeenSet = false;

    StackInstanceComprehensiveStatus m_stackInstanceStatus;
    bool m_stackInstanceStatusHasBeenSet = false;

    Aws::String m_statusReason;
    bool m_statusReasonHasBeenSet = false;

    Aws::String m_organizationalUnitId;
    bool m_organizationalUnitIdHasBeenSet = false;

    StackDriftStatus m_driftStatus;
    bool m_driftStatusHasBeenSet = false;

    Aws::Utils::DateTime m_lastDriftCheckTimestamp;
    bool m_lastDriftCheckTimestampHasBeenSet = false;

    Aws::String m_lastOperationId;
    bool m_lastOperationIdHasBeenSet = false;
  };

} // namespace Model
} // namespace CloudFormation
} // namespace Aws
