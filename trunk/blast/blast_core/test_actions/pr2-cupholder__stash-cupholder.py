
class BlastPr2StashCupHolderActionExec(BlastActionExec):
    def __init__(self):
        BlastActionExec.__init__(self)
    def run(self, parameters):
        self.robot_transfer_holder("left-arm", "cupholder")
        self.set_robot_position("left-arm", [0.0, -0.350, 0.0, -1.225, 3.14159, -1.65, 0.0, False])
set_action_exec(BlastPr2StashCupHolderActionExec)