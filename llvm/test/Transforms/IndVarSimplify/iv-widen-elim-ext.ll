; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -indvars -S | FileCheck %s

target datalayout = "e-m:e-i64:64-p:64:64:64-n8:16:32:64-S128"

; When widening IV and its users, trunc and zext/sext are not needed
; if the original 32-bit user is known to be non-negative, whether
; the IV is considered signed or unsigned.
define void @foo(i32* %A, i32* %B, i32* %C, i32 %N) {
; CHECK-LABEL: @foo(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[CMP1:%.*]] = icmp slt i32 0, %N
; CHECK-NEXT:    br i1 [[CMP1]], label %for.body.lr.ph, label %for.end
; CHECK:       for.body.lr.ph:
; CHECK-NEXT:    br label %for.body
; CHECK:       for.body:
; CHECK-NEXT:    [[INDVARS_IV:%.*]] = phi i64 [ [[INDVARS_IV:%.*]].next, %for.inc ], [ 0, %for.body.lr.ph ]
; CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i32, i32* %B, i64 [[INDVARS_IV]]
; CHECK-NEXT:    [[TMP0:%.*]] = load i32, i32* [[ARRAYIDX]], align 4
; CHECK-NEXT:    [[TMP1:%.*]] = add nuw nsw i64 [[INDVARS_IV]], 2
; CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds i32, i32* %C, i64 [[TMP1]]
; CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32* [[ARRAYIDX2]], align 4
; CHECK-NEXT:    [[ADD3:%.*]] = add nsw i32 [[TMP0]], [[TMP2]]
; CHECK-NEXT:    [[TRUNC0:%.*]] = trunc i64 [[TMP1]] to i32
; CHECK-NEXT:    [[DIV0:%.*]] = udiv i32 5, [[TRUNC0]]
; CHECK-NEXT:    [[ADD4:%.*]] = add nsw i32 [[ADD3]], [[DIV0]]
; CHECK-NEXT:    [[ARRAYIDX5:%.*]] = getelementptr inbounds i32, i32* %A, i64 [[INDVARS_IV]]
; CHECK-NEXT:    store i32 [[ADD4]], i32* [[ARRAYIDX5]], align 4
; CHECK-NEXT:    br label %for.inc
; CHECK:       for.inc:
; CHECK-NEXT:    [[INDVARS_IV_NEXT:%.*]] = add nuw nsw i64 [[INDVARS_IV]], 1
; CHECK-NEXT:    [[WIDE_TRIP_COUNT:%.*]] = zext i32 %N to i64
; CHECK-NEXT:    [[EXITCOND:%.*]] = icmp ne i64 [[INDVARS_IV_NEXT]], [[WIDE_TRIP_COUNT]]
; CHECK-NEXT:    br i1 [[EXITCOND]], label %for.body, label %for.cond.for.end_crit_edge
; CHECK:       for.cond.for.end_crit_edge:
; CHECK-NEXT:    br label %for.end
; CHECK:       for.end:
; CHECK-NEXT:    ret void
;
entry:
  %cmp1 = icmp slt i32 0, %N
  br i1 %cmp1, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.inc
  %i.02 = phi i32 [ 0, %for.body.lr.ph ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds i32, i32* %B, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %add = add nsw i32 %i.02, 2
  %idxprom1 = zext i32 %add to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %C, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %add3 = add nsw i32 %0, %1
  %div0 = udiv i32 5, %add
  %add4 = add nsw i32 %add3, %div0
  %idxprom4 = zext i32 %i.02 to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %A, i64 %idxprom4
  store i32 %add4, i32* %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc, %N
  br i1 %cmp, label %for.body, label %for.cond.for.end_crit_edge

for.cond.for.end_crit_edge:                       ; preds = %for.inc
  br label %for.end

for.end:                                          ; preds = %for.cond.for.end_crit_edge, %entry
  ret void
}

define void @foo1(i32* %A, i32* %B, i32* %C, i32 %N) {
; CHECK-LABEL: @foo1(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[CMP1:%.*]] = icmp slt i32 0, %N
; CHECK-NEXT:    br i1 [[CMP1]], label %for.body.lr.ph, label %for.end
; CHECK:       for.body.lr.ph:
; CHECK-NEXT:    br label %for.body
; CHECK:       for.body:
; CHECK-NEXT:    [[INDVARS_IV:%.*]] = phi i64 [ [[INDVARS_IV:%.*]].next, %for.inc ], [ 0, %for.body.lr.ph ]
; CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i32, i32* %B, i64 [[INDVARS_IV]]
; CHECK-NEXT:    [[TMP0:%.*]] = load i32, i32* [[ARRAYIDX]], align 4
; CHECK-NEXT:    [[TMP1:%.*]] = add nuw nsw i64 [[INDVARS_IV]], 2
; CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds i32, i32* %C, i64 [[TMP1]]
; CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32* [[ARRAYIDX2]], align 4
; CHECK-NEXT:    [[ADD3:%.*]] = add nsw i32 [[TMP0]], [[TMP2]]
; CHECK-NEXT:    [[ARRAYIDX5:%.*]] = getelementptr inbounds i32, i32* %A, i64 [[INDVARS_IV]]
; CHECK-NEXT:    store i32 [[ADD3]], i32* [[ARRAYIDX5]], align 4
; CHECK-NEXT:    br label %for.inc
; CHECK:       for.inc:
; CHECK-NEXT:    [[INDVARS_IV_NEXT:%.*]] = add nuw nsw i64 [[INDVARS_IV]], 1
; CHECK-NEXT:    [[WIDE_TRIP_COUNT:%.*]] = zext i32 %N to i64
; CHECK-NEXT:    [[EXITCOND:%.*]] = icmp ne i64 [[INDVARS_IV_NEXT]], [[WIDE_TRIP_COUNT]]
; CHECK-NEXT:    br i1 [[EXITCOND]], label %for.body, label %for.cond.for.end_crit_edge
; CHECK:       for.cond.for.end_crit_edge:
; CHECK-NEXT:    br label %for.end
; CHECK:       for.end:
; CHECK-NEXT:    ret void
;
entry:
  %cmp1 = icmp slt i32 0, %N
  br i1 %cmp1, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.inc
  %i.02 = phi i32 [ 0, %for.body.lr.ph ], [ %inc, %for.inc ]
  %idxprom = zext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds i32, i32* %B, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %add = add nsw i32 %i.02, 2
  %idxprom1 = sext i32 %add to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %C, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %add3 = add nsw i32 %0, %1
  %idxprom4 = sext i32 %i.02 to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %A, i64 %idxprom4
  store i32 %add3, i32* %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc, %N
  br i1 %cmp, label %for.body, label %for.cond.for.end_crit_edge

for.cond.for.end_crit_edge:                       ; preds = %for.inc
  br label %for.end

for.end:                                          ; preds = %for.cond.for.end_crit_edge, %entry
  ret void
}


@a = common global [100 x i32] zeroinitializer, align 16
@b = common global [100 x i32] zeroinitializer, align 16

define i32 @foo2(i32 %M) {
; CHECK-LABEL: @foo2(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[CMP1:%.*]] = icmp slt i32 0, %M
; CHECK-NEXT:    br i1 [[CMP1]], label %for.body.lr.ph, label %for.end
; CHECK:       for.body.lr.ph:
; CHECK-NEXT:    [[TMP0:%.*]] = sext i32 %M to i64
; CHECK-NEXT:    br label %for.body
; CHECK:       for.body:
; CHECK-NEXT:    [[INDVARS_IV:%.*]] = phi i64 [ [[INDVARS_IV:%.*]].next, %for.inc ], [ 0, %for.body.lr.ph ]
; CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 [[INDVARS_IV]]
; CHECK-NEXT:    [[TMP1:%.*]] = load i32, i32* [[ARRAYIDX]], align 4
; CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds [100 x i32], [100 x i32]* @b, i64 0, i64 [[INDVARS_IV]]
; CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32* [[ARRAYIDX2]], align 4
; CHECK-NEXT:    [[ADD:%.*]] = add nsw i32 [[TMP1]], [[TMP2]]
; CHECK-NEXT:    [[TMP3:%.*]] = add nsw i64 [[INDVARS_IV]], [[TMP0]]
; CHECK-NEXT:    [[ARRAYIDX5:%.*]] = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 [[TMP3]]
; CHECK-NEXT:    store i32 [[ADD]], i32* [[ARRAYIDX5]], align 4
; CHECK-NEXT:    br label %for.inc
; CHECK:       for.inc:
; CHECK-NEXT:    [[INDVARS_IV_NEXT:%.*]] = add nuw nsw i64 [[INDVARS_IV]], 1
; CHECK-NEXT:    [[WIDE_TRIP_COUNT:%.*]] = zext i32 %M to i64
; CHECK-NEXT:    [[EXITCOND:%.*]] = icmp ne i64 [[INDVARS_IV_NEXT]], [[WIDE_TRIP_COUNT]]
; CHECK-NEXT:    br i1 [[EXITCOND]], label %for.body, label %for.cond.for.end_crit_edge
; CHECK:       for.cond.for.end_crit_edge:
; CHECK-NEXT:    br label %for.end
; CHECK:       for.end:
; CHECK-NEXT:    [[CALL:%.*]] = call i32 @dummy(i32* getelementptr inbounds ([100 x i32], [100 x i32]* @a, i32 0, i32 0), i32* getelementptr inbounds ([100 x i32], [100 x i32]* @b, i32 0, i32 0))
; CHECK-NEXT:    ret i32 0
;
entry:
  %cmp1 = icmp slt i32 0, %M
  br i1 %cmp1, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.inc
  %i.02 = phi i32 [ 0, %for.body.lr.ph ], [ %inc, %for.inc ]
  %idxprom = zext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %i.02 to i64
  %arrayidx2 = getelementptr inbounds [100 x i32], [100 x i32]* @b, i64 0, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %add = add nsw i32 %0, %1
  %add3 = add nsw i32 %i.02, %M
  %idxprom4 = sext i32 %add3 to i64
  %arrayidx5 = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 %idxprom4
  store i32 %add, i32* %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc, %M
  br i1 %cmp, label %for.body, label %for.cond.for.end_crit_edge

for.cond.for.end_crit_edge:                       ; preds = %for.inc
  br label %for.end

for.end:                                          ; preds = %for.cond.for.end_crit_edge, %entry
  %call = call i32 @dummy(i32* getelementptr inbounds ([100 x i32], [100 x i32]* @a, i32 0, i32 0), i32* getelementptr inbounds ([100 x i32], [100 x i32]* @b, i32 0, i32 0))
  ret i32 0
}

declare i32 @dummy(i32*, i32*)

; A case where zext should not be eliminated when its operands could only be extended by sext.
define i32 @foo3(i32 %M) {
; CHECK-LABEL: @foo3(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[CMP1:%.*]] = icmp slt i32 0, %M
; CHECK-NEXT:    br i1 [[CMP1]], label %for.body.lr.ph, label %for.end
; CHECK:       for.body.lr.ph:
; CHECK-NEXT:    [[TMP0:%.*]] = sext i32 %M to i64
; CHECK-NEXT:    br label %for.body
; CHECK:       for.body:
; CHECK-NEXT:    [[INDVARS_IV:%.*]] = phi i64 [ [[INDVARS_IV:%.*]].next, %for.inc ], [ 0, %for.body.lr.ph ]
; CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 [[INDVARS_IV]]
; CHECK-NEXT:    [[TMP1:%.*]] = load i32, i32* [[ARRAYIDX]], align 4
; CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds [100 x i32], [100 x i32]* @b, i64 0, i64 [[INDVARS_IV]]
; CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32* [[ARRAYIDX2]], align 4
; CHECK-NEXT:    [[ADD:%.*]] = add nsw i32 [[TMP1]], [[TMP2]]
; CHECK-NEXT:    [[TMP3:%.*]] = add nsw i64 [[INDVARS_IV]], [[TMP0]]
; CHECK-NEXT:    [[TMP4:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[IDXPROM4:%.*]] = zext i32 [[TMP4]] to i64
; CHECK-NEXT:    [[ARRAYIDX5:%.*]] = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 [[IDXPROM4]]
; CHECK-NEXT:    store i32 [[ADD]], i32* [[ARRAYIDX5]], align 4
; CHECK-NEXT:    br label %for.inc
; CHECK:       for.inc:
; CHECK-NEXT:    [[INDVARS_IV_NEXT:%.*]] = add nuw nsw i64 [[INDVARS_IV]], 1
; CHECK-NEXT:    [[WIDE_TRIP_COUNT:%.*]] = zext i32 %M to i64
; CHECK-NEXT:    [[EXITCOND:%.*]] = icmp ne i64 [[INDVARS_IV_NEXT]], [[WIDE_TRIP_COUNT]]
; CHECK-NEXT:    br i1 [[EXITCOND]], label %for.body, label %for.cond.for.end_crit_edge
; CHECK:       for.cond.for.end_crit_edge:
; CHECK-NEXT:    br label %for.end
; CHECK:       for.end:
; CHECK-NEXT:    [[CALL:%.*]] = call i32 @dummy(i32* getelementptr inbounds ([100 x i32], [100 x i32]* @a, i32 0, i32 0), i32* getelementptr inbounds ([100 x i32], [100 x i32]* @b, i32 0, i32 0))
; CHECK-NEXT:    ret i32 0
;
entry:
  %cmp1 = icmp slt i32 0, %M
  br i1 %cmp1, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.inc
  %i.02 = phi i32 [ 0, %for.body.lr.ph ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %i.02 to i64
  %arrayidx2 = getelementptr inbounds [100 x i32], [100 x i32]* @b, i64 0, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %add = add nsw i32 %0, %1
  %add3 = add nsw i32 %i.02, %M
  %idxprom4 = zext i32 %add3 to i64
  %arrayidx5 = getelementptr inbounds [100 x i32], [100 x i32]* @a, i64 0, i64 %idxprom4
  store i32 %add, i32* %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc, %M
  br i1 %cmp, label %for.body, label %for.cond.for.end_crit_edge

for.cond.for.end_crit_edge:                       ; preds = %for.inc
  br label %for.end

for.end:                                          ; preds = %for.cond.for.end_crit_edge, %entry
  %call = call i32 @dummy(i32* getelementptr inbounds ([100 x i32], [100 x i32]* @a, i32 0, i32 0), i32* getelementptr inbounds ([100 x i32], [100 x i32]* @b, i32 0, i32 0))
  ret i32 0
}
